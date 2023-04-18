# FTP client
[![version](https://img.shields.io/badge/version-0.0.1-blue.svg)](https://github.com/EthanAndreas/FtpClient)
[![compiler](https://img.shields.io/badge/compiler-gcc-red.svg)](https://github.com/EthanAndreas/FtpClient/blob/main/Makefile)
[![author](https://img.shields.io/badge/author-EthanAndreas-blue)](https://github.com/EthanAndreas)
[![author](https://img.shields.io/badge/author-Cottelle-blue)](https://github.com/Cottelle)

## Asbtract 

This is a simple network client that can be used to test the network server.

## Functionnality

- `open @IP` : Connect to a server at the given IP address
- `quit` : Disconnect from the server
- `dir` : List the files in the server's directory
- `get @file` : Get a file from the server
- `ren @file @newname` : Rename a file on the server

## Command

- Compilation

- Launch
```bash
./bin/client 
```
> For direct connection to a server, use the IP address as an argument:
```bash
./bin/client @IP
```
