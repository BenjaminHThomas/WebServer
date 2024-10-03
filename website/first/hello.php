#!/usr/bin/php-cgi
<?php
header("Content-Type: text/html");

echo "<!DOCTYPE html>";
echo "<html lang='en'>";
echo "<head>";
echo "<meta charset='UTF-8'>";
echo "<title>Hello World CGI</title>";
echo "</head>";
echo "<body>";
echo "<h1>Hello World from PHP CGI!</h1>";
echo "<p>Current date and time: " . date('Y-m-d H:i:s') . "</p>";
// echo "<p>Server name: " . $_SERVER['SERVER_NAME'] . "</p>";
// echo "<p>CGI version: " . $_SERVER['GATEWAY_INTERFACE'] . "</p>";
// echo "<p>Client IP: " . $_SERVER['REMOTE_ADDR'] . "</p>";
echo "</body>";
echo "</html>";
?>