#if !defined MAX_RDIALOG_PAGES
    #define MAX_RDIALOG_PAGES        200 

#endif

#if !defined MAX_RDIALOG_PAGESLENGTH
    #define MAX_RDIALOG_PAGESLENGTH  128 

#endif

#define DIALOG_PAGES_ID          8888

#define DialogPages:%0(%1) \
    forward rdlgpgs_%0(%1); public rdlgpgs_%0(%1)

new bool:rdialogpages_ListitemExist[MAX_PLAYERS][MAX_RDIALOG_PAGES];

new rdialogpages_ListitemString[MAX_PLAYERS][MAX_RDIALOG_PAGES][MAX_RDIALOG_PAGESLENGTH char]; 

new rdialogpages_Function[MAX_PLAYERS][32];
new rdialogpages_Style[MAX_PLAYERS];
new rdialogpages_Caption[MAX_PLAYERS][64];
new rdialogpages_Button1[MAX_PLAYERS][32]; 

new rdialogpages_Button2[MAX_PLAYERS][32];
new rdialogpages_ItemPerPage[MAX_PLAYERS];
new rdialogpages_NextButton[MAX_PLAYERS][64];
new rdialogpages_BackButton[MAX_PLAYERS][64];
new rdialogpages_OpenedPage[MAX_PLAYERS];

stock ClearPlayerDialogPages(playerid)
{
    for (new i = 0; i < MAX_RDIALOG_PAGES; i++)
    {

        rdialogpages_ListitemExist[playerid][i] = false; 
    }
    rdialogpages_OpenedPage[playerid] = 0;
    rdialogpages_Function[playerid][0] = '\0';
    return 1;
}

stock AddPlayerDialogPages(playerid, const listitem[])
{
    new rdialogpages_slot = -1;
    for(new i = 0; i < MAX_RDIALOG_PAGES; i++)
    {
        if(!rdialogpages_ListitemExist[playerid][i])
        {
            rdialogpages_slot = i;
            break;
        }
    }

    if(rdialogpages_slot == -1)
    {
        printf("[rDialogPages] WARNING: Player %d mencapai batas maksimal item dialog (%d)!", playerid, MAX_RDIALOG_PAGES);
        return 0;
    }

    new cleaned[MAX_RDIALOG_PAGESLENGTH];
    new idx = 0;

    for (new i = 0; listitem[i] != '\0' && idx < MAX_RDIALOG_PAGESLENGTH - 1; i++)
    {
        if (listitem[i] == '\n' || listitem[i] == '\r') continue;
        cleaned[idx++] = listitem[i];
    }
    cleaned[idx] = '\0';

    rdialogpages_ListitemExist[playerid][rdialogpages_slot] = true;

    strpack(rdialogpages_ListitemString[playerid][rdialogpages_slot], cleaned, MAX_RDIALOG_PAGESLENGTH);
    return 1;
}

stock rDialogPages_GetMaxPage(playerid)
{
    new totalItems = 0;
    for (new i = 0; i < MAX_RDIALOG_PAGES; i++)
    {
        if (rdialogpages_ListitemExist[playerid][i]) totalItems++;
    }

    new pageSize = rdialogpages_ItemPerPage[playerid];
    if (pageSize <= 0) return 0;

    new bool:hasHeader = (rdialogpages_Style[playerid] == _:DIALOG_STYLE_TABLIST_HEADERS) && (rdialogpages_ListitemExist[playerid][0]);
    new dataItems = totalItems - (hasHeader ? 1 : 0);

    if (dataItems <= 0) return 0;

    return (dataItems + pageSize - 1) / pageSize;
}

stock rDialogPages_OpenPage(playerid, page) 
{
    new maxPage = rDialogPages_GetMaxPage(playerid);
    if (maxPage == 0) return 0;

    if (page < 1) page = 1;
    if (page > maxPage) page = maxPage;

    new per = rdialogpages_ItemPerPage[playerid];
    new bool:hasHeader = (rdialogpages_Style[playerid] == _:DIALOG_STYLE_TABLIST_HEADERS) && (rdialogpages_ListitemExist[playerid][0]);

    new startIndex = (page - 1) * per + (hasHeader ? 1 : 0);
    new endIndex   = startIndex + per - 1;

    new chunk[2048], caption[128], tempString[MAX_RDIALOG_PAGESLENGTH]; 
    chunk[0] = '\0';

    if (hasHeader)
    {
        strunpack(tempString, rdialogpages_ListitemString[playerid][0], sizeof(tempString));
        strcat(chunk, tempString);
        strcat(chunk, "\n");
    }

    for (new i = startIndex; i <= endIndex && i < MAX_RDIALOG_PAGES; i++)
    {
        if(!rdialogpages_ListitemExist[playerid][i]) continue;

        strunpack(tempString, rdialogpages_ListitemString[playerid][i], sizeof(tempString));
        strcat(chunk, tempString);
        strcat(chunk, "\n");
    }

    if(page == 1 && maxPage > 1)
    {
        strcat(chunk, rdialogpages_NextButton[playerid]);
        strcat(chunk, "\n");
    }
    else if(page == maxPage && page > 1)
    {
        strcat(chunk, rdialogpages_BackButton[playerid]);
        strcat(chunk, "\n");
    }
    else if(page > 1 && page < maxPage)
    {
        strcat(chunk, rdialogpages_NextButton[playerid]);
        strcat(chunk, "\n");
        strcat(chunk, rdialogpages_BackButton[playerid]);
        strcat(chunk, "\n");
    }

    rdialogpages_OpenedPage[playerid] = page;

    format(caption, sizeof(caption), "%s (%d/%d)", rdialogpages_Caption[playerid], page, maxPage);
    ShowPlayerDialog(playerid, DIALOG_PAGES_ID, rdialogpages_Style[playerid], caption, chunk, rdialogpages_Button1[playerid], rdialogpages_Button2[playerid]);
    return 1;
}

stock ShowPlayerDialogPages(playerid, const function[], style, const caption[], const button1[], const button2[], item_per_page = 10, const nextbutton[] = "{FF0000}Halaman Selanjutnya >>>", const backbutton[] = "{FF0000}<<< Halaman Sebelumnya")
{
    format(rdialogpages_Function[playerid], sizeof(rdialogpages_Function[]), "rdlgpgs_%s", function);
    format(rdialogpages_Caption[playerid], sizeof(rdialogpages_Caption[]), "%s", caption);
    format(rdialogpages_Button1[playerid], sizeof(rdialogpages_Button1[]), "%s", button1);
    format(rdialogpages_Button2[playerid], sizeof(rdialogpages_Button2[]), "%s", button2);
    format(rdialogpages_NextButton[playerid], sizeof(rdialogpages_NextButton[]), "%s", nextbutton);
    format(rdialogpages_BackButton[playerid], sizeof(rdialogpages_BackButton[]), "%s", backbutton);

    if(item_per_page <= 0) item_per_page = 10;
    rdialogpages_ItemPerPage[playerid] = item_per_page;
    rdialogpages_Style[playerid] = style;

    return rDialogPages_OpenPage(playerid, 1);
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_PAGES_ID)
    {
        if(!response)
        {
            ClearPlayerDialogPages(playerid);

            #if defined rdlgpgs_OnDialogResponse
                return rdlgpgs_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
            #else
                return 1;
            #endif
        }

        new page = rdialogpages_OpenedPage[playerid];
        new per  = rdialogpages_ItemPerPage[playerid];
        new maxPage = rDialogPages_GetMaxPage(playerid);

        new totalItems = 0;
        for(new i = 0; i < MAX_RDIALOG_PAGES; i++)
            if(rdialogpages_ListitemExist[playerid][i]) totalItems++;

        new bool:hasHeader = (rdialogpages_Style[playerid] == _:DIALOG_STYLE_TABLIST_HEADERS) && (rdialogpages_ListitemExist[playerid][0]);
        new dataTotal = totalItems - (hasHeader ? 1 : 0);

        new itemsOnThisPage;
        if(page < maxPage) itemsOnThisPage = per;
        else
        {
            itemsOnThisPage = dataTotal - (maxPage - 1) * per;
            if(itemsOnThisPage < 0) itemsOnThisPage = 0;
        }

        if(listitem < itemsOnThisPage)
        {
            new base = (page - 1) * per + (hasHeader ? 1 : 0);
            new realIndex = base + listitem;

            if(rdialogpages_ListitemExist[playerid][realIndex])
            {
                if(rdialogpages_Function[playerid][0] != '\0')
                {
                    CallLocalFunction(rdialogpages_Function[playerid], "ddds", playerid, response, realIndex, inputtext);
                    ClearPlayerDialogPages(playerid);
                }
            }
        }
        else
        {
            if(page == 1 && maxPage > 1) rDialogPages_OpenPage(playerid, page + 1);
            else if(page == maxPage && page > 1) rDialogPages_OpenPage(playerid, page - 1);
            else
            {
                if(listitem == itemsOnThisPage) rDialogPages_OpenPage(playerid, page + 1);
                else if(listitem == itemsOnThisPage + 1) rDialogPages_OpenPage(playerid, page - 1);
            }
        }
        return 1; 

    }

    #if defined rdlgpgs_OnDialogResponse
        return rdlgpgs_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    #else
        return 0;
    #endif
}

#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif

#define OnDialogResponse rdlgpgs_OnDialogResponse

#if defined rdlgpgs_OnDialogResponse
    forward rdlgpgs_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif
