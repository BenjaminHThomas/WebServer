import os
import sys

sys.stdout.write("Content-type: text/html\r\n\r\n");
print()  # Blank line to separate headers from body

print("<html>")
print("<body>")
print("<font size=+1>Environment</font><br>")
for param in os.environ.keys():
   print("<b>%20s</b>: %s<br>" % (param, os.environ[param]))
print("</body>")
print("</html>")