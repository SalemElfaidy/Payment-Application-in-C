#include <stdio.h>
#include "server.h"
#include <string.h>

ST_accountsDB_t database [255]={{0,500,"4117394584032808"},
                                {0,12000,"5370233590092076"},
                                {0,5000,"5078034830297201"}};

ST_transaction_t transactions [255] = {0};

float temp_current_card_balance = -1;
uint8_t temp_current_card_index = -1;

void updateBalance (uint8_t i , float transAmount){

    database[i].balance -= transAmount;
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData){

    if(isValidAccount(&transData->cardHolderData) != OK){
        transData->transState = DECLINED_STOLEN_CARD;
        saveTransaction(transData);
        return DECLINED_STOLEN_CARD;
    }

    if(isAmountAvailable(&transData->terminalData) != OK){
        transData->transState = DECLINED_INSUFFECIENT_FUND;
        saveTransaction(transData);
        return DECLINED_INSUFFECIENT_FUND;
    }

    updateBalance(temp_current_card_index , transData->terminalData.transAmount);

    transData->transState = APPROVED;

    if(saveTransaction(transData) != OK){
        return INTERNAL_SERVER_ERROR;
    }

    temp_current_card_index = -1;
    temp_current_card_balance = -1;

    return APPROVED;


}

EN_serverError_t isValidAccount(ST_cardData_t *cardData){

    for (int i = 0; i < 255; i++) {

        if(strcmp(database[i].primaryAccountNumber , cardData->primaryAccountNumber) == 0){

            if(database[i].isBlocked){
                return DECLINED_STOLEN_CARD;
            }

            temp_current_card_balance = database[i].balance;
            temp_current_card_index = i;

            return OK;
        }
    }

    return DECLINED_STOLEN_CARD;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData){

    if(temp_current_card_balance > 0 && termData->transAmount <= temp_current_card_balance){
        return OK;
    }

    return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData){

    for (int i = 0; i < 255; i++) {

        //looking for an Empty place to add the new transaction
        if(transactions[i].transactionSequenceNumber == 0 && transactions[i].cardHolderData.primaryAccountNumber[0] == '\0'){

            transactions[i] = *transData;
            transData->transactionSequenceNumber = i;

            return OK;
        }
    }

    return SAVING_FAILED;
    
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData){

    if(transactionSequenceNumber < 0 || transactionSequenceNumber > 255){
        return ACCOUNT_NOT_FOUND;
    }

    *transData = transactions[transactionSequenceNumber];

    return OK;
}
