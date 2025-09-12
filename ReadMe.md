# MCP Server Template

A minimal C++ MCP server supporting:
- Raw TCP socket (custom protocol)
- HTTP (for browser/web clients)
- CoAP (UDP, for IoT/embedded)

## Configuration

Edit `config.json` to set ports:
```json
{
  "tcp_port": 5000,
  "http_port": 8080,
  "coap_port": 5683
}
```

## Build

```sh
sudo apt update
sudo apt install build-essential cmake
git clone <repo-url> mcp-server
cd mcp-server
cmake -B build
cmake --build build
```

## Run

```sh
cd build
./mcp_server
```

## Deploy as .deb

1. Edit `debian/control` for package info.
2. Build the binary as above.
3. From project root:
   ```sh
   dpkg-deb --build . mcp-server.deb
   ```
4. Install:
   ```sh
   sudo dpkg -i mcp-server.deb
   ```

## Notes

- TCP: Connect with `telnet localhost 5000`
- HTTP: Visit `http://localhost:8080`
- CoAP: Use a CoAP client (e.g., `coap-client`)