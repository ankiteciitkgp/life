import sqlite3 as lite
import sys

con = lite.connect('User.db')

with con:

    cur = con.cursor()
    cur.execute("CREATE TABLE Users(Roll TEXT, Room TEXT, Product TEXT,Password TEXT, Key TEXT);")
    cur.execute("CREATE UNIQUE INDEX keyx on Users (Key);")
