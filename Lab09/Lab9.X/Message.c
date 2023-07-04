/*====================\
    Mathias Eriksen    
    mterikse@ucsc.edu
    ECE13 
    Lab09                  
\=====================*/

#include "Message.h"
#include "BOARD.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t Message_CalculateChecksum(const char* payload){
    
    int i;
    uint8_t Checksum = 0;
    // from lab doc, XOR each bit
    for (i=0; i<(strlen(payload)); i++){
        Checksum ^= payload[i];
    }
    
    return Checksum;   
}

int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event){    
    // check correct length
    if (strlen(checksum_string) != MESSAGE_CHECKSUM_LEN){
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    // calculate and check checksum value    
    uint8_t Checksum = Message_CalculateChecksum(payload);
    char ChecksumString[MESSAGE_CHECKSUM_LEN];
    sprintf(ChecksumString, "%02X", Checksum);
    
    if (strcmp(ChecksumString, checksum_string) != 0){
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    // break apart string and check first part
    char * parse_payload = malloc(strlen(payload));   
    strcpy(parse_payload, payload);
    char * token;   
    
    token = strtok(parse_payload, ",\n ");
    // first if checks first part, following strtok() take string apart further and
    // saves parameters
    if (strcmp(token, "CHA") == 0){
        token = strtok(NULL, ",\n ");
        uint16_t paramvalCHA = atoi(token);
        message_event->param0 = paramvalCHA;
        
        message_event->type = BB_EVENT_CHA_RECEIVED;       
    }
    else if (strcmp(token, "ACC") == 0){
        token = strtok(NULL, ",\n ");
        uint16_t paramvalACC = atoi(token);
        message_event->param0 = paramvalACC;
        
        message_event->type = BB_EVENT_ACC_RECEIVED;        
    }
    else if (strcmp(token, "REV") == 0){
        token = strtok(NULL, ",\n ");
        uint16_t paramvalREV = atoi(token);
        message_event->param0 = paramvalREV;
        
        message_event->type = BB_EVENT_REV_RECEIVED;        
    }
    else if (strcmp(token, "SHO") == 0){
        token = strtok(NULL, ",\n ");
        int paramvalSHO1 = atoi(token);
        message_event->param0 = paramvalSHO1;
        
        token = strtok(NULL, ",\n ");
        int paramvalSHO2 = atoi(token);
        message_event->param1 = paramvalSHO2;
        
        message_event->type = BB_EVENT_SHO_RECEIVED;        
    }
    else if (strcmp(token, "RES") == 0){
        token = strtok(NULL, ",\n ");
        uint16_t paramvalRES1 = atoi(token);
        message_event->param0 = paramvalRES1;
        
        token = strtok(NULL, ",\n ");
        uint16_t paramvalRES2 = atoi(token);
        message_event->param1 = paramvalRES2;
        
        token = strtok(NULL, ",\n ");
        uint16_t paramvalRES3 = atoi(token);
        message_event->param2 = paramvalRES3;
        
        message_event->type = BB_EVENT_RES_RECEIVED;        
    } 
    else {
        return STANDARD_ERROR;
    }
    // free memeory for copy of payload
    free(parse_payload);
    return SUCCESS;
}

int Message_Encode(char *message_string, Message message_to_encode){
    
    char payload[MESSAGE_MAX_PAYLOAD_LEN];
    sprintf(message_string, "%c", "$");
    // switch based on message type
    switch (message_to_encode.type){
        // use templates
        case (MESSAGE_CHA):
            sprintf(payload, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
            break;
        case (MESSAGE_ACC):
            sprintf(payload, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
            break;
        case (MESSAGE_REV):
            sprintf(payload, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
            break;
        case (MESSAGE_SHO):
            sprintf(payload, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
            break;
        case (MESSAGE_RES):
            sprintf(payload, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1, message_to_encode.param2);
            break;  
        case (MESSAGE_ERROR):
            return 0;            
    }
    
    uint8_t Checksum;
    Checksum = Message_CalculateChecksum(payload);
    
    char ChecksumString[MESSAGE_CHECKSUM_LEN];
    sprintf(ChecksumString, "%02X", Checksum);
    // compile all elements of string
    sprintf(message_string, "%c", '$');
    strcat(message_string, payload);
    strcat(message_string, "*");
    strcat(message_string, ChecksumString);
    strcat(message_string, "\n");
    
    return strlen(message_string);
}
// statics since multiple calls will be made to this function
DecodeState State = WAITING;

static char payload[MESSAGE_MAX_PAYLOAD_LEN];
static char checksum[MESSAGE_CHECKSUM_LEN];

static int payloadindex;
static int checksumindex;

int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event){
    
    switch (State){
        // waiting for start delimiter
        case (WAITING):
            
            if (char_in == '$'){
                payloadindex = 0;
                checksumindex = 0;
                State = RECORDING_PAYLOAD;
            }
            break;
       // recording payload betweeen delimiters     
        case (RECORDING_PAYLOAD):
            
            if (payloadindex > MESSAGE_MAX_PAYLOAD_LEN){
                State = WAITING;
                return STANDARD_ERROR;
            } else {
                
                if (char_in == '\n'){
                    payload[payloadindex] = '\0';                   
                    State = WAITING;
                    return STANDARD_ERROR;
                }
                else if (char_in == '$'){
                    payload[payloadindex] = '\0';                   
                    State = WAITING;
                    return STANDARD_ERROR;
                }
                else if (char_in == '*'){
                    payload[payloadindex++] = '\0';
                    State = RECORDING_CHECKSUM;                    
                }
            // sets payload string to passed char
            payload[payloadindex++] = char_in;
            }            
            break;
            
        case (RECORDING_CHECKSUM):
            
            if (checksumindex > MESSAGE_CHECKSUM_LEN + 1) {
                State = WAITING;
                //printf("Error 4");
                return STANDARD_ERROR;   
            } else {
                
                if (('0' <= char_in <= '9') | ('A' <= char_in <= 'F')) {  
                    checksum[checksumindex++] = char_in;                                       
                } 
                else if (char_in == '\n'){
                    checksum[checksumindex++] = '\0';
                    if (Message_ParseMessage(payload,checksum,decoded_message_event) == SUCCESS){
                        State = WAITING;                       
                    }
                } else {
                    //printf("Error 5");
                    State = WAITING;
                    return STANDARD_ERROR;
                }                   
            }
            
            return SUCCESS;
            break;
    }
}