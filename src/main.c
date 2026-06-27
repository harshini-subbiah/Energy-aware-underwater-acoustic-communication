#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>

/* ----------- SYSTEM PARAMETERS ----------- */
#define CHUNK_SIZE 4
#define BITRATE 100.0

#define PERIODIC_POWER 10
#define EVENT_POWER 25
#define BURST_POWER 50

#define POWER_MARGIN 2

#define DISTANCE 5.0   // distance (meters)

/* ----------- BELLHOP CHANNEL PARAMETERS ----------- */
#define BELLHOP_TL          12.86   /* Transmission Loss from BELLHOP (dB)  */
#define AMBIENT_NOISE_LEVEL 50.0    /* Shallow water ambient noise (dB)     */

/* ----------- GLOBAL POWER (Battery Simulation) ----------- */
double available_energy;

/* ----------- BELLHOP HELPER FUNCTIONS ----------- */
double power_to_SL(double tx_power_watts) {
    /* Source Level in dB re 1 uPa */
    return 170.0 + 10.0 * log10(tx_power_watts);
}

double compute_SNR(double tx_power_watts) {
    double SL  = power_to_SL(tx_power_watts);
    double SNR = SL - BELLHOP_TL - AMBIENT_NOISE_LEVEL;
    return SNR;
}

int compute_loss_probability(double SNR) {
    if (SNR > 20.0) return 5;
    if (SNR > 10.0) return 20;
    if (SNR > 0.0)  return 50;
    return 85;
}

/* ----------- CHANNEL-AWARE RECEIVER (BELLHOP-based) ----------- */
int receiver_receive(const char *data, char *buffer, double tx_power) {

    double SNR  = compute_SNR(tx_power);
    int    loss = compute_loss_probability(SNR);

    printf("[RX] TL=%.2f dB | SNR=%.2f dB | Loss Probability=%d%%\n",
           BELLHOP_TL, SNR, loss);

    if ((rand() % 100) < loss) {
        printf("[RX] Packet LOST (BELLHOP channel model)\n");
        return 0;
    }

    strcat(buffer, data);
    printf("[RX] Received: %s\n", data);
    return 1;
}

/* ----------- PERIODIC ----------- */
void periodic_transmit(const char *msg) {
    char buffer[100] = "";
    char chunk[CHUNK_SIZE + 1];
    int len = strlen(msg);
    double energy = 0;

    printf("\n--- PERIODIC MODE ---\n");
    int i;
    for (i = 0; i < len; i += CHUNK_SIZE) {
        strncpy(chunk, msg + i, CHUNK_SIZE);
        chunk[CHUNK_SIZE] = '\0';

        int ack = 0;
        while (!ack) {
            printf("[TX | %dW] %s\n", PERIODIC_POWER, chunk);
            Sleep(500);

            double e = (strlen(chunk) * PERIODIC_POWER) / BITRATE;
            energy += e;
            available_energy -= e;

            ack = receiver_receive(chunk, buffer, PERIODIC_POWER);

            if (!ack) printf("[TX] Retransmitting...\n");
        }
    }

    printf("[RX] Final: %s\nEnergy Used: %.3f J\n", buffer, energy);
}

/* ----------- BURST ----------- */
void burst_transmit(const char *msg) {
    char buffer[100] = "";
    double energy;

    printf("\n--- BURST MODE ---\n");

    printf("[TX | %dW] Sending full message\n", BURST_POWER);
    Sleep(300);

    energy = (strlen(msg) * BURST_POWER) / BITRATE;
    available_energy -= energy;

    receiver_receive(msg, buffer, BURST_POWER);

    printf("[RX] Final: %s\nEnergy Used: %.3f J\n", buffer, energy);
}

/* ----------- EVENT ----------- */
void event_transmit(const char *msg) {
    char buffer[100] = "";
    double energy;

    printf("\n--- EVENT MODE ---\n");
    printf("[EVENT] Trigger detected\n");

    printf("[TX | %dW] %s\n", EVENT_POWER, msg);
    Sleep(400);

    energy = (strlen(msg) * EVENT_POWER) / BITRATE;
    available_energy -= energy;

    int ack = receiver_receive(msg, buffer, EVENT_POWER);

    if (!ack)
        printf("[TX] Event retransmission skipped (energy saving)\n");

    printf("[RX] Final: %s\nEnergy Used: %.3f J\n", buffer, energy);
}

/* ----------- ADAPTIVE TRANSMIT ----------- */
void adaptive_transmit(const char *msg, int event_flag) {

    printf("\nAvailable Power: %.2f W\n", available_energy);

    printf("[CHECK] Event condition: %s\n",
        (event_flag && available_energy >= (EVENT_POWER + POWER_MARGIN)) ? "TRUE" : "FALSE");

    printf("[CHECK] Burst condition: %s\n",
        (available_energy >= (BURST_POWER + POWER_MARGIN)) ? "TRUE" : "FALSE");

    printf("[CHECK] Periodic condition: %s\n",
        (available_energy >= (PERIODIC_POWER + POWER_MARGIN)) ? "TRUE" : "FALSE");

    /* Show BELLHOP channel info before transmitting */
    printf("\n[BELLHOP] Channel Info at %.1fm range:\n", DISTANCE);
    printf("  Transmission Loss : %.2f dB\n", BELLHOP_TL);
    printf("  Noise Floor       : %.1f dB\n", AMBIENT_NOISE_LEVEL);
    printf("  SNR (PERIODIC/%dW): %.2f dB\n", PERIODIC_POWER, compute_SNR(PERIODIC_POWER));
    printf("  SNR (EVENT/%dW)   : %.2f dB\n", EVENT_POWER,    compute_SNR(EVENT_POWER));
    printf("  SNR (BURST/%dW)   : %.2f dB\n", BURST_POWER,    compute_SNR(BURST_POWER));

    if (event_flag && available_energy >= (EVENT_POWER + POWER_MARGIN)) {
        printf(">> SELECTED: EVENT MODE\n");
        event_transmit(msg);
    }
    else if (available_energy >= (BURST_POWER + POWER_MARGIN)) {
        printf(">> SELECTED: BURST MODE\n");
        burst_transmit(msg);
    }
    else if (available_energy >= (PERIODIC_POWER + POWER_MARGIN)) {
        printf(">> SELECTED: PERIODIC MODE\n");
        periodic_transmit(msg);
    }
    else {
        printf("\n[TX] Not enough power - System Idle\n");
    }

    printf("\n[Battery] Remaining Power: %.2f W\n", available_energy);
}

/* ----------- MAIN ----------- */
int main() {
    srand(time(NULL));

    const char *message = "ACOUSTIC_COMM";
    int event_flag;

    printf("==============================================\n");
    printf("  Adaptive Underwater Acoustic Communication \n");
    printf("  Channel Model: BELLHOP Ray Tracing         \n");
    printf("==============================================\n");
    printf("BELLHOP TL = %.2f dB at %.1fm range\n\n", BELLHOP_TL, DISTANCE);

    printf("Enter initial available energy (J): ");
    scanf("%lf", &available_energy);

    while (1) {
        printf("\n--- NEW TRANSMISSION CYCLE ---\n");

        printf("Enter event (1 = YES, 0 = NO, -1 to exit): ");
        scanf("%d", &event_flag);

        if (event_flag == -1) {
            printf("Exiting system...\n");
            break;
        }

        adaptive_transmit(message, event_flag);

        Sleep(1000);
    }

    return 0;
}
