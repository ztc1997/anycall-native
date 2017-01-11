# anycall-native
This project can run as root to call system service RPC from normal apps.

## Build
1. Download Android source code, see to https://source.android.com/source/downloading.html.
2. `cd ${SourceDir>}`
3. `source build/envsetup.sh`
4. `lunch`
5. `make`
6. Wait for compiling, I suggest U have a good sleep.
7. `cd external`
8. `git clone https://github.com/ztc1997/anycall-native.git`
9. `cd BinderDemo`
10. `mm`
11. The binary should be located in `${SourceDir}/out/target/product/*/system/bin/anycall`.

## Use
### Loop
Start shell and it will connect to service `anycall ${service name}`  
Input format `${transaction code} ${Base64.encodeToString(data.marshall())}`.  

### Single line
Input `anycall ${service name} ${transaction code} ${Base64.encodeToString(data.marshall())}`  
Output format `${Base64.encodeToString(reply.marshall())}`.  
The `data`, `reply` type is `android.os.Parcel`.