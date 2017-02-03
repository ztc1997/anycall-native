/*
 * Copyright 2016 xiaokangwang
 * Copyright 2016-2017 Alex Zhang aka. ztc1997
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define LOG_TAG "anycall"

#include <stdlib.h>

#include <binder/IBinder.h>
#include <binder/BpBinder.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include "base64.h"
#include "retval.h"

using namespace android;

char *callMethod(sp<IBinder> binder, int code, char *encodedDate)
{
    decode_result result = base64_decode(encodedDate, strlen(encodedDate));

    Parcel data, reply;

    data.setDataSize(result.ret_len);
    data.setDataPosition(0);

    void *raw = data.writeInplace(result.ret_len);
    memmove(raw, result.ret, result.ret_len); 

    status_t st = binder->remoteBinder()->transact(code, data, &reply);

    char *output = base64_encode(reply.data(), reply.dataSize());

    return output;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Missing parameters.\n");
        return ERROR_MISSING_PARAMETERS;
    }

    String16 token = String16(argv[1]);
    sp<IServiceManager> sm = defaultServiceManager();
    if (sm == 0)
    {
        printf("Failed to get ServiceManager.\n");
        return ERROR_FAILED_TO_GET_SERVICE_MANAGER;
    }

    sp<IBinder> binder = sm->getService(String16(token));
    if (binder == 0)
    {
        printf("Failed to get service, the service may not be running.\n");
        return ERROR_FAILED_TO_GET_SERVICE;
    }

    if (argc == 4)
    {
        int code = atoi(argv[2]);
        char *encodedDate = argv[3];

        char *output = callMethod(binder, code, encodedDate);
        printf("%s\n", output);

        return 0;
    }

    for (;;)
    {
        char input[256];
        fgets(input, sizeof(input), stdin);

        if (strcmp(strtok(input, "\n"), "exit") == 0)
            break;

        int code = atoi(strtok(input, " "));
        char *encodedDate = strtok(NULL, "\n");

        char *output = callMethod(binder, code, encodedDate);
        printf("%s\n", output);
    }

    return 0;
}
