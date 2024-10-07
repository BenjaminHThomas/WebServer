#!/usr/bin/env python3

import cgi
import os
from http import cookies
import datetime
import sys

def print_headers(cookie=None):
    print("Content-Type: text/html")
    if cookie:
        print(cookie.output())
    print()  # blank line to end headers

def print_html(content):
    print("<html><body>")
    print(content)
    print("</body></html>")

def create_form():
    return """
    <form method="POST">
        Username: <input type="text" name="username"><br>
        Email: <input type="text" name="email"><br>
        <input type="submit" value="Submit">
    </form>
    """

def create_welcome_page(username, email):
    return f"""
    <h1>Welcome, {username}!</h1>
    <p>Your email is: {email}</p>
    <p>This information is stored in cookies.</p>
    <button onclick="clearCookies()">Clear Cookies</button>
    <script>
    function clearCookies() {{
        fetch(window.location.href, {{
            method: 'DELETE',
        }}).then(() => {{
            window.location.reload();
        }});
    }}
    </script>
    """

def clear_cookies():
    cookie = cookies.SimpleCookie()
    cookie["username"] = ""
    cookie["email"] = ""
    for morsel in cookie.values():
        morsel["expires"] = "Thu, 01 Jan 1970 00:00:00 GMT"
    return cookie

def handle_delete():
    new_cookie = clear_cookies()
    print_headers(new_cookie)
    print_html("Cookies cleared. <a href='/'>Go back</a>")

def handle_get(cookie):
    if "username" in cookie and "email" in cookie:
        print_headers()
        print_html(create_welcome_page(cookie["username"].value, cookie["email"].value))
    else:
        print_headers()
        print_html(create_form())

def handle_post(form):
    if "username" in form and "email" in form:
        username = form["username"].value
        email = form["email"].value

        new_cookie = cookies.SimpleCookie()
        new_cookie["username"] = username
        new_cookie["email"] = email
        for morsel in new_cookie.values():
            morsel["expires"] = (datetime.datetime.now() + datetime.timedelta(days=30)).strftime("%a, %d-%b-%Y %H:%M:%S GMT")

        print_headers(new_cookie)
        print_html(create_welcome_page(username, email))
    else:
        print_headers()
        print_html(create_form())

def main():
    method = os.environ.get("REQUEST_METHOD", "GET")
    cookie = cookies.SimpleCookie(os.environ.get("HTTP_COOKIE", ""))

    if method == "DELETE":
        handle_delete()
    elif method == "GET":
        handle_get(cookie)
    elif method == "POST":
        form = cgi.FieldStorage()
        handle_post(form)
    else:
        print("Status: 405 Method Not Allowed")
        print_headers()
        print_html("Method Not Allowed")

if __name__ == "__main__":
    main()