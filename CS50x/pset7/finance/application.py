from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

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
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Show user's portfolio"""
    # Route accessed via link (or redirect)
    if request.method == 'GET':
        
        # Query current user's data (from users table) and stocks (from transactions tables). Select transactions of current user
        udata = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        tr = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE username = :user GROUP BY symbol", user=udata[0]["username"])        
        
        # Create lists to store subtotals (shares * current price), dictonaries of stocks and current price
        subtotals = []
        stocks = []
        price = 0
        
        # Iterate over each stock obtained from database, use lookup to obtain price and name.
        for stock in tr:
            temp = lookup(stock["symbol"])
            if not temp:
                return "Unable to fetch data"
            
            # Calculate subtotal (current price * number of shares), append result to a list
            price = temp["price"]
            subtotal = price * stock["SUM(shares)"]
            subtotals.append(subtotal)
            
            # If user has shares of stock store results on a dictionary, format values as USD and append it to list.
            if stock["SUM(shares)"] > 0:
                tempdict = {"symbol": temp["symbol"], "name": temp["name"], "shares": stock["SUM(shares)"], "price": usd(price), "subtotal": usd(subtotal)}
                stocks.append(tempdict)
        
        # Calculate total value stocks and grand total by adding user's cash.    
        sumofstocks = sum(subtotals)
        grandtotal = usd(sumofstocks + udata[0]["cash"])
            
        return render_template("index.html", stocks = stocks, cash = usd(udata[0]["cash"]), grandtotal = grandtotal)
        

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    # route accessed via link
    if request.method == "GET":
        return render_template("buy.html")
    
    # route accessed via POST (form)    
    if request.method == "POST":
        
        # ensure correct usage
        if not request.form.get("symbol"):
            return apology("Provide stock's symbol")
            
        if not str.isdigit(request.form.get("shares")):
            return apology("How many shares?")
        
        symbol = lookup(request.form.get("symbol"))
        if not symbol:
            return apology("Invalid symbol")
        shares = request.form.get("shares")
        
        # Calculate cost of select shares and query information (current user)
        cost = symbol['price'] * int(shares)
        row = db.execute("SELECT * FROM users WHERE id = :userid", userid=session["user_id"])
        
        # Check if cost of shares is greater than user's funds. Register transaction in database if user has money.
        if cost > row[0]["cash"]:
            return apology("Insufficient funds")
        else:
            t = db.execute("INSERT INTO transactions (username, symbol, shares, price, operation) VALUES (:username, :symbol, :shares, :price, 'B')", 
            username=row[0]["username"], symbol=symbol["symbol"], shares=int(shares), price=symbol["price"])
            
            c = db.execute("UPDATE users SET cash = cash - :cost WHERE id = :userid", cost=cost, userid=session["user_id"])
            return redirect(url_for("index"))
    

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    if request.method == "GET":
        
        user = db.execute("SELECT * FROM users WHERE id = :id", id = session["user_id"])
        query = db.execute("SELECT * from transactions WHERE username = :user", user = user[0]["username"])
        
        stocks = []
        
        for stock in query:
            if stock["operation"] == 'S':
                temp = 'Sold'
            else:
                temp = 'Bought'
            
            tempdict = {"symbol": stock["symbol"],"shares": abs(stock["shares"]),"price": usd(stock["price"]),"operation": temp, "date": stock["date"]}
            stocks.append(tempdict)
            
    return render_template("history.html", stocks = stocks)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # route accessed via links
    if request.method == "GET":
        return render_template("quote.html")
    
    # route accessed via POST (form)
    if request.method == "POST":
        
        # Store user's input (symbol), look it up and then store in a dict
        symbol = request.form.get("symbol")
        quoted = lookup(symbol)
        
        # If the symbol does not exist (lookup will return nothing) display a error message. Otherwise, print stock's price.
        if not quoted:
            return apology("Incorret symbol")
        else:
            return render_template("quoted.html", name = quoted['name'], symbol = quoted['symbol'], price = usd(quoted['price'])) 
    

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # route accessed via links
    if request.method == "GET":
        return render_template("register.html")
    
    # route accessed via post (form)
    if request.method == "POST":
        
        # ensure all fields are filled returning a error otherwise
        if not request.form.get("username"):
            return apology("Provide username")

        elif not request.form.get("password") or not request.form.get("password2"):
            return apology("Provide (and confirm) password!")
        
        elif request.form.get("password") != request.form.get("password2"):
            return apology("Passwords must match!")
        
        # hash user's password
        hash = pwd_context.hash(request.form.get("password"))
        
        # Insert user into database
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=hash)
        if not result:
            return apology("Unable to register")
        # If registration is successful get user's id and store on session to login the user.
        else:
            userid = db.execute("SELECT id FROM users WHERE username=:username", username=request.form.get("username"))
            session["user_id"] = userid[0]["id"]
            return redirect(url_for("index"))
        

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    # Route accessed via link 
    if request.method == "GET":
        return render_template("sell.html")
    
    # Route accessed via POST (form)
    if request.method == "POST":
        
        # Ensure fields are correctly filled
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Provide stock symbol and number of shares")
        
        if not str.isdigit(request.form.get("shares")):
            return apology("How many shares?")
    
        # Convert symbol to uppercase and shares 'number' to integer
        symbol = str.upper(request.form.get("symbol"))
        shares = int(request.form.get("shares"))

        # Quote
        stockd = lookup(symbol)
        if not stockd:
            return apology("Incorrect symbol")
        
        # Check if user has selected stocks, print apology otherwise        
        user = db.execute("SELECT * FROM users WHERE id = :id", id = session["user_id"])    
        query = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE username = :user AND symbol = :symbol GROUP BY symbol", 
        symbol = symbol, user = user[0]["username"])
        
        if not query:
            return apology("No stocks")
        
        # Obtain sum of shares, check if user has the inputed number of shares and proceed to decrease the qtd of shares.
        sumshares = int(query[0]["SUM(shares)"])
        if sumshares < shares:
            return apology("Insufficient shares")
        else:
            ins = db.execute("INSERT INTO transactions (username, symbol, shares, price, operation) VALUES (:user, :symbol, :shares, :price, 'S')", 
            user = user[0]["username"], symbol = symbol, shares = -shares, price = stockd["price"])
            
            # Calculate the current price of sold shares
            cost = shares * float(stockd["price"])
            
            # update cash
            updatecash = db.execute("UPDATE users SET cash = cash + :cost WHERE id = :userid", cost=cost, userid=session["user_id"])
            return redirect(url_for("index"))

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Change user's password."""
    
    # Route accessed via link
    if request.method == "GET":
        return render_template("change.html")
    
    # Route accessed via POST (form)
    if request.method == "POST":
        
        # Ensure fields are correct filled
        if not request.form.get("current"):
            return apology("Provide current password")
            
        elif not request.form.get("password") or not request.form.get("password2"):
            return apology("Provide (and confirm) password!")
        
        elif request.form.get("password") != request.form.get("password2"):
            return apology("Passwords must match!")

        # Hash new password
        new = pwd_context.hash(request.form.get("password"))
        
        # Query DB to check for current password (hash)
        user = db.execute("SELECT * FROM users WHERE id = :id", id = session["user_id"])
        
        # Check if current password is correct 
        if not pwd_context.verify(request.form.get("current"), user[0]["hash"]):
            return apology("Wrong password")
        else:
            # Update user's password with new hash, returning to index
            new = db.execute("UPDATE users SET hash = :new WHERE id = :userid", new=new, userid=session["user_id"])
            return redirect(url_for("index"))
            
        
    
            