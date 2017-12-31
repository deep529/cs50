# from cs50 import SQL
import sqlite3
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# connect to database, define cursor
db = sqlite3.connect("finance.db")
crsr = db.cursor()

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
#db = SQL("sqlite:///finance.db")


# ---------------------------------------------REGISTER---------------------------------------------
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    # if request method is GET, load register.html page
    if request.method == "GET":
        return render_template("register.html")

    # else submit the form
    else:
        if not request.form.get("username"):
            return error("wow! you don't want to provide username")
        elif not request.form.get("password"):
            return error("wow! you don't want to keep password")
        elif not request.form.get("password2"):
            return error("dude! Re-enter the password")
        elif request.form.get("password") != request.form.get("password2"):
            return error("Password did not match")

        # look if username exist
        crsr.execute("SELECT * FROM users WHERE username=(?)", (request.form.get("username"),) )

        # if exist then return error
        if crsr.fetchone() != None:
            return error("Username already exist")
        else:
            # add username and hash password in database
            crsr.execute("INSERT INTO users (username, hash) VALUES ((?), (?));",
                ( request.form.get("username"), pwd_context.encrypt(request.form.get("password")), ) )
            db.commit()
            return redirect(url_for("index"))


# ---------------------------------------------LOGIN---------------------------------------------
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return error("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return error("must provide password")

        # query database for username
        crsr.execute("SELECT * FROM users WHERE username = (?);", (request.form.get("username"),) )
        row = crsr.fetchone()

        # ensure username exists and password is correct
        if row==None or not pwd_context.verify(request.form.get("password"), row[2]):
            return error("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = row[0]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


# ---------------------------------------------INDEX---------------------------------------------
@app.route("/")
@login_required
def index():
    symbol = []
    share = []
    price = []

    crsr.execute("SELECT symbol,newHolding FROM history WHERE id=(?) ORDER BY time DESC;", (session["user_id"],) )
    transactionHistory = crsr.fetchall()
    total = 0.0

    for row in transactionHistory:
        if int(row[1])>0 and row[0] not in symbol:
            symbol.append(row[0])
            share.append(int(row[1]))
            currentPrice = lookup(row[0])
            price.append( round(currentPrice["price"],2) )
            total = total + int(row[1])*currentPrice["price"]

    crsr.execute("SELECT cash FROM users WHERE id=(?);", (session["user_id"],) )
    cash = crsr.fetchone()
    cash = cash[0]
    cash = round(cash, 2)
    total = total + cash
    total = round(total, 2)

    return render_template("index.html", symbol=symbol, share=share, price=price, size=len(symbol), cash=cash, total=total)


# ---------------------------------------------QUOTE---------------------------------------------
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        quoteResult = lookup(request.form.get("symbol"))

        if quoteResult == None:
            return error("Error occurred in retrieving cost")
        else:
            return render_template("quoted.html", symbol=quoteResult["symbol"], name=quoteResult["name"], price=quoteResult["price"])


# ---------------------------------------------BUY---------------------------------------------
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # Buy shares of stock

    if request.method == "GET":
        # rendering buying page
        return render_template("buy.html")
    else:
        # buying stocks
        quoteResult = lookup(request.form.get("symbol"))

        # if error occured in retrieving cost
        if quoteResult == None:
            return error("Error occurred in retrieving cost")

        crsr.execute("SELECT cash FROM users WHERE id=(?);", (session["user_id"],) )
        money = crsr.fetchone()
        money = money[0]

        print("balance:{} bill:{}".format(money, float(request.form.get("quantity"))*float(quoteResult["price"]) ))

        # abort if user do not have enough money
        if money < float(request.form.get("quantity"))*float(quoteResult["price"]):
            print("Don't have enough cash")
            return error("Don't have enough cash")

        crsr.execute("SELECT newHolding FROM history WHERE id=(?) AND symbol=(?) ORDER BY time DESC;", (session["user_id"], request.form.get("symbol"),) )
        prevData = crsr.fetchone()

        if prevData == None:
            prevHolding = 0
        else:
            prevHolding = int(prevData[0])

        print("prevHolding={}".format(prevHolding))

        crsr.execute("INSERT INTO history (id, symbol, rate, quantity, newHolding) VALUES ((?), (?), (?), (?), (?));", \
            (session["user_id"], request.form.get("symbol"), float(quoteResult["price"]), int(request.form.get("quantity")), int(request.form.get("quantity"))+prevHolding,) )

        print("updated cash={}".format(money-float(request.form.get("quantity"))*float(quoteResult["price"]) ) )
        crsr.execute("UPDATE users SET cash=(?) WHERE id=(?);", (money-float(request.form.get("quantity"))*float(quoteResult["price"]), session["user_id"],) )
        db.commit()
        return error("SUCCESSFULLY BOUGHT")

# ---------------------------------------------SELL---------------------------------------------
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    return error("TODO")


# ---------------------------------------------HISTORY---------------------------------------------
@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return error("TODO")


# ---------------------------------------------LOGOUT---------------------------------------------
@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))