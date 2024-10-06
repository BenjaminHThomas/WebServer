#!/usr/bin/env python3

import cgi, sys

form = cgi.FieldStorage()

sys.stdout.write("Content-type: text/html\r\n\r\n")
print("<html>")
print("<head><title>Upload Results</title></head>")
print("<body>")

print("<h1>Upload Results</h1>")
print("<p>Here are the values you submitted:</p>")

print("<table>")
print("<tr><th>Field</th><th>Value</th></tr>")
print("<tr><td>Name</td><td>{}</td></tr>".format(form.getvalue("name")))
print("<tr><td>Project</td><td>{}</td></tr>".format(form.getvalue("project")))
print("<tr><td>Description</td><td>{}</td></tr>".format(form.getvalue("description")))
print("</table>")

print("</body>")
print("</html>")