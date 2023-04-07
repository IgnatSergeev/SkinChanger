#include <windows.h>
#include <stdio.h>
#include "SkinChanger.h"
#include <stdbool.h>

/*
*  __  __     ______     ______     ______        __     __   __     ______   ______     ______     ______   ______     ______     ______
* /\ \/\ \   /\  ___\   /\  ___\   /\  == \      /\ \   /\ "-.\ \   /\__  _\ /\  ___\   /\  == \   /\  ___\ /\  __ \   /\  ___\   /\  ___\
* \ \ \_\ \  \ \___  \  \ \  __\   \ \  __<      \ \ \  \ \ \-.  \  \/_/\ \/ \ \  __\   \ \  __<   \ \  __\ \ \  __ \  \ \ \____  \ \  __\
*  \ \_____\  \/\_____\  \ \_____\  \ \_\ \_\     \ \_\  \ \_\\"\_\    \ \_\  \ \_____\  \ \_\ \_\  \ \_\    \ \_\ \_\  \ \_____\  \ \_____\
*   \/_____/   \/_____/   \/_____/   \/_/ /_/      \/_/   \/_/ \/_/     \/_/   \/_____/   \/_/ /_/   \/_/     \/_/\/_/   \/_____/   \/_____/
*
*/
void PrintMenu(const char* title, char** names, UINT numberOfItems, UINT currentItemIndex)
{
    bool isLastItem = currentItemIndex == numberOfItems - 1;
    bool isFirstItem = currentItemIndex == 0;

    printf("%s %c %s %c\t\t\t\r", title, isFirstItem ? '|' : '<', names[currentItemIndex], isLastItem ? '|' : '>');
    Sleep(150);// For get async key state not to skip some items
}

enum ItemDefinitionIndex ItemSelect(const char* title, char** names, UINT numberOfItems)
{
    UINT currentItemIndex = 0;
    PrintMenu(title, names, numberOfItems, currentItemIndex);

    while (!GetAsyncKeyState(VK_RETURN))
    {
        bool isLastItem = currentItemIndex == numberOfItems - 1;
        bool isFirstItem = currentItemIndex == 0;

        if (GetAsyncKeyState(VK_RIGHT) && !isLastItem)
        {
            PrintMenu(title, names, numberOfItems, ++currentItemIndex);
        }
        else if (GetAsyncKeyState(VK_LEFT) && !isFirstItem)
        {
            PrintMenu(title, names, numberOfItems, --currentItemIndex);
        }
    }

    printf("%s %s\t\t\t\n", title, names[currentItemIndex]);
    Sleep(150);// In case after this function called one that uses get async
    return currentItemIndex;
}

UINT LoadSkins(const char *path, char ***skinNames, UINT **skinIds)
{
    FILE *skinsFile = fopen(path, "r");
    if (skinsFile == NULL)
    {
        return 0;
    }

    UINT currentArraysSize = 0;

    char line[64] = {};
    while (fgets(line, sizeof(line), skinsFile))
    {

        UINT lineSize = strlen(line);

        char idString[64] = {};
        char nameString[64] = {};
        UINT nameStringStartPosition = 0;
        bool isReadingId = true;

        // Reading id:
        for (int i = 0; i < lineSize; i++)
        {
            if (line[i] == '\0' || line[i] == '\n')
            {
                break;
            }

            if (line[i] == ':')
            {
                isReadingId = false;

                // Storing id:
                (*skinIds) = (UINT *)realloc(*skinIds, (currentArraysSize + 1) * sizeof(UINT));

                UINT id = strtol(idString, NULL, 10);
                (*skinIds)[currentArraysSize] = id;

                nameStringStartPosition = i + 2;
                break;
            }

            idString[i] = line[i];
        }

        // Reading name:
        for (UINT i = nameStringStartPosition; i < lineSize; i++)
        {
            if (line[i] == '\0' || line[i] == '\n')
            {
                break;
            }
            nameString[i - nameStringStartPosition] = line[i];
        }

        // Storing name:
        (*skinNames) = (char **)realloc(*skinNames, (currentArraysSize + 1) * sizeof(char *));
        (*skinNames)[currentArraysSize] = strdup(nameString);

        ++currentArraysSize;
    }

    fclose(skinsFile);
    return 0;
}

void SkinChanger()
{
    char** skinNames = NULL;
    UINT* skinIDs = NULL;


    UINT numOfSkins = LoadSkins("../skins.txt", &skinNames, &skinIDs);
    enum ItemDefinitionIndex knife = knifeIDs[ItemSelect("[+] Knife model:", knifeNames, NUM_OF_KNIVES)];

    if (skinNames)
    {
        free(skinNames);
    }
    if (skinIDs)
    {
        free(skinIDs);
    }
}

int main()
{
    SkinChanger();

}