# FTP client

This is a simple network client that can be used to test the network server.

## Commands

- `open @IP` : Connect to a server at the given IP address
- `quit` : Disconnect from the server
- `dir` : List the files in the server's directory
- `get @file` : Get a file from the server
- `ren @file @newname` : Rename a file on the server

## Usage

```bash
./bin/client 
```

For direct connection to a server, use the IP address as an argument:

```bash
./bin/client @IP
```

## Contributors

- [Ethan Huret]()
- [Maxime Collette]()
