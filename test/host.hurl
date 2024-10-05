GET http://localhost:7777
HTTP 200
[Asserts]
body contains "<p>Hello there! We're glad you're here.</p>"

GET http://okoca.42.fr:7777
HTTP 200
[Asserts]
body contains "<p>A random image courtesy of picsum.photos.com.</p>"
