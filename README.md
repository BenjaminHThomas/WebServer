# WebServer

WebServer is an HTTP/1.1 server implemented from scratch in C++98, designed to handle HTTP requests, responses, and dynamic content generation through CGI scripts.

## Features

- **HTTP Protocol Support**: Handles standard HTTP methods and responses.
- **Static File Serving**: Efficiently delivers static content to clients.
- **CGI Support**: Executes Common Gateway Interface scripts for dynamic content generation.
- **Error Handling**: Provides custom error pages for various HTTP status codes.
- **Configurable Design**: Easily customizable via configuration files.
- **Unit Testing**: Includes unit tests to ensure code reliability.
- **HURL Integration**: Uses HURL for advanced HTTP testing and validation.

## Getting Started

### Using Makefile

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/BenjaminHThomas/WebServer.git
   ```
2. **Build the Server**:
   ```bash
   cd webserver
   make
   ```
3. **Run the Server**:
   ```bash
   ./webserver <config_filename>.json
   ```

### Using Docker Compose

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/BenjaminHThomas/WebServer.git
   ```
2. **Build and Run the Server**:
   ```bash
   cd webserver
   docker-compose up --build
   ```

## Configuration

The server requires a configuration file to run. Examples are provided in the `config` directory, such as `config.json`. These files define server parameters, routes, and CGI script paths.

## Testing

- **Unit Tests**: Run `make test` to execute the suite of unit tests, ensuring functionality and stability.
- **HURL Testing**: Utilize HURL scripts to perform comprehensive HTTP tests. These validate server behavior under various scenarios.

## Usage

After starting the server with a configuration file, access it via the specified host and port in the configuration file. For example, if the configuration sets the server to listen on `127.0.0.1:8080`, access it through `http://127.0.0.1:8080` or `http://localhost:8080`.

## License

This project is licensed under the MIT License.
