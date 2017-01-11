/*
 * Copyright 2016 xiaokangwang
 * Copyright 2016 Alex Zhang aka. ztc1997
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
#include <iostream>

#include <binder/IBinder.h>
#include <binder/BpBinder.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include "base64.h"

using namespace std;
using namespace android;

string callMethod(sp<IBinder> binder, int code, string encodedDate)
{
    string dataBytes = base64_decode(encodedDate);
    unsigned int dataLength = dataBytes.length();

    Parcel data, reply;

    data.setDataSize(dataLength);
    data.setDataPosition(0);

    void *raw = data.writeInplace(dataLength);
    memcpy(raw, dataBytes.c_str(), dataLength);

    status_t st = binder->remoteBinder()->transact(code, data, &reply);

    std::string output = base64_encode(reinterpret_cast<const unsigned char *>(reply.data()), reply.dataSize());

    return output;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Please enter a service name." << endl;
        return 1;
    }

    String16 token = String16(argv[1]);
    sp<IServiceManager> sm = defaultServiceManager();
    if (sm == 0)
    {
        cout << "Failed to get ServiceManager." << endl;
        return 2;
    }

    sp<IBinder> binder = sm->getService(String16(token));
    if (binder == 0)
    {
        cout << "Failed to get service, the service may not be running." << endl;
        return 3;
    }

    if (argc == 4)
    {
        int code = atoi(argv[2]);
        string encodedDate = argv[3];

        string output = callMethod(binder, code, encodedDate);
        cout << output << endl;

        return 0;
    }

    for (;;)
    {
        string input;
        getline(cin, input);

        if (input == "exit")
            break;

        unsigned long index = input.find(" ");
        int code = atoi(input.substr(0, index).c_str());
        string encodedDate = input.substr(index + 1, input.length());

        string output = callMethod(binder, code, encodedDate);
        cout << output << endl;
    }
    return 0;
}
