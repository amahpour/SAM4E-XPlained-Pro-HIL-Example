/*
 * main.c
 *
 * Created: 02/07/2020
 * Author: Ari Mahpour
 */

#include <asf.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 50

void processCommand(char* command);

// Variable declarations used within main
char commandBuffer[MAX_COMMAND_LENGTH];
int commandBufferCount = 0;

int main(void)
{
    // Calling SAM4E Xplained Pro board initialization function
    board_init();

    // Call a local utility routine to initialize C-Library Standard I/O over
    // a USB CDC protocol. Tunable parameters in a conf_usb.h file must be
    // supplied to configure the USB device correctly.
    stdio_usb_init();

    memset(commandBuffer, 0, sizeof commandBuffer);
    commandBufferCount = 0;

    // Run this loop forever
    while (1) {
        // Wait on input over USB CDC interface
        // Currently using TeraTerm as the debug tool
        while (udi_cdc_get_nb_received_data()) {
            // Retrieve command char
            char inputChar = udi_cdc_getc();

            // Reset commandBuffer
            if (inputChar == '\r' || inputChar == '\n') {
                processCommand(commandBuffer);
                memset(commandBuffer, 0, sizeof commandBuffer);
                commandBufferCount = 0;
            }
            else {
                // Add to commandBuffer and increment buffer character counter
                commandBuffer[commandBufferCount] = inputChar;
                commandBufferCount++;
            }
        }
    }
}

void processCommand(char* command)
{
    printf("Processing command: %s\r\n", command);

    if (strcmp(command, "gpnvmclr") == 0) {
        printf("Clearing GP NVM.\r\n");
        flash_clear_gpnvm(1);
    }
    else {
        printf("Command not recognized.\r\n");
    }
}
