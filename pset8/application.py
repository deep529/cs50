import os
import re
from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue

# from cs50 import SQL
import sqlite3
from helpers import lookup

# configure application
app = Flask(__name__)
JSGlue(app)

# connect to database & define cursor
db = sqlite3.connect("mashup.db")
cur = db.cursor()

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

@app.route("/")
def index():
    """Render map."""

    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))

@app.route("/articles", methods=["GET", "POST"])
def articles():
    """Look up articles for geo."""

    # get geolocation parameter
    geo = request.args.get("geo","")

    # look for news
    news = lookup(geo)
    return jsonify(news)

@app.route("/search", methods=["GET", "POST"])
def search():
    """Search for places that match query."""

    # UNDER PROGRESS

    # get search query
    # 1st argument is key, second argument is default value if key(given as 1st argument) doesn't exist
    query = request.args.get("q", "")

    result = []

    if query.isdecimal():
        # find matches where query can be substring anywhere placed in main string
        query = "%" + query + "%"
        cur.execute("SELECT * FROM places WHERE postal_code LIKE (?);", (query,) )
        matches = cur.fetchall()

        # remove % from string
        query = query[1:-1]

        # if query has first letter 0 then find all items starting with query(but without first 0)
        # because if 0213 is query then 2138 result can't be find since sql ignores preceding zeros
        if query[0] == '0' and query!='0':
            # remove 0 from string
            query = query[1:]
            query = query + "%"

            cur.execute("SELECT * FROM places WHERE postal_code LIKE (?);", (query,) )
            extra = cur.fetchall()

            for element in extra:
                matches.append(element)

        for match in matches:
            result_element = {}

            result_element["country_code"] = match[0]
            result_element["postal_code"] = match[1]
            result_element["place_name"] = match[2]
            result_element["admin_name1"] = match[3]
            result_element["admin_code1"] = match[4]
            result_element["admin_name2"] = match[5]
            result_element["admin_code2"] = match[6]
            result_element["admin_name3"] = match[7]
            result_element["admin_code3"] = match[8]
            result_element["latitude"] = match[9]
            result_element["longitude"] = match[10]
            result_element["accuracy"] = match[11]

            result.append(result_element)

    return jsonify(result)

@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        command = "SELECT * FROM places WHERE " + \
                  ":sw_lat <= latitude AND latitude <= :ne_lat AND " + \
                  ":sw_lng <= longitude AND longitude <= :ne_lng " + \
                  "GROUP BY country_code, place_name, admin_code1 ORDER BY RANDOM() LIMIT 10"

        cur.execute(command, (sw_lat, ne_lat, sw_lng, ne_lng,) )
        rows = cur.fetchall()

    else:

        # crosses the antimeridian
        command = "SELECT * FROM places WHERE " + \
                  "(?) <= latitude AND latitude <= (?) AND " + \
                  "( (?) <= longitude OR longitude <= (?) ) " + \
                  "GROUP BY country_code, place_name, admin_code1 ORDER BY RANDOM() LIMIT 10"

        cur.execute(command, (sw_lat, ne_lat, sw_lng, ne_lng,) )
        rows = cur.fetchall()

    # output places as JSON
    return jsonify(rows)
