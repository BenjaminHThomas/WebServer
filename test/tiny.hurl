# Normal get request with 0 body size
GET http://localhost:8080
HTTP 200
[Asserts]
body contains "<p>Hello there! We're glad you're here.</p>"

# At the limit (pretty sure it's including \r\n in char count)
GET http://localhost:8081
Content-Type: text/plain
Content-Length: 3
Custom-Header: test
"012"

HTTP 200
[Asserts]
body contains "<h1>THIS IS THE ABOUT PAGE!</h1>"

# Over the limit of 5
GET http://localhost:7778
Content-Type: text/plain
Content-Length: 11
Custom-Header: test
"01234567890"

HTTP 413
[Asserts]
header "Content-Type" contains "text/html"
body contains "<html><body><h2>Oops! Got an error: </h2><h1>413 Payload Too Large</h1></body></html>"


