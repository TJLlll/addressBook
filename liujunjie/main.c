#include <stdio.h>
#include "choiseFuncManu.h"
#include "addressBookInit.h"

int main(int argc, char const *argv[])
{
    addressGetData contactsData = addressBookInit();
    if (!contactsData)
    {
        perror("malloc error");
    }

    switchFuncManu(contactsData);

    return 0;
}
