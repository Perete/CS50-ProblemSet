import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    # Obtener las acciones que posee el usuario desde la base de datos
    user_id = session["user_id"]
    rows = db.execute(
        "SELECT symbol, SUM(shares) AS total_shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0", user_id)

    # Obtener el saldo de efectivo del usuario
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    # Crear una lista para almacenar los datos del portafolio
    portfolio = []
    total_value = cash

    # Recorrer cada acción que el usuario posee
    for row in rows:
        symbol = row["symbol"]
        shares = row["total_shares"]

        # Obtener el precio actual de la acción
        stock = lookup(symbol)
        if stock is None:
            return apology("error with stock lookup", 400)

        price = stock["price"]
        total_stock_value = shares * price

        # Sumar el valor total de las acciones al total general
        total_value += total_stock_value

        # Agregar los datos de la acción al portafolio
        portfolio.append({
            "symbol": symbol,
            "name": stock["name"],
            "shares": shares,
            "price": price,
            "total": total_stock_value
        })

    # Renderizar la página principal con los datos del portafolio y efectivo
    return render_template("index.html", portfolio=portfolio, cash=cash, total_value=total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        # Verificar que los campos no estén vacíos y que las acciones sean positivas
        if not symbol:
            return apology("must provide symbol", 400)
        if not shares.isdigit() or int(shares) <= 0:
            return apology("must provide positive number of shares", 400)

        # Llamar a lookup para obtener la cotización
        stock = lookup(symbol)
        if stock is None:
            return apology("invalid symbol", 400)

        # Verificar que el usuario tiene suficiente dinero
        price_per_share = stock["price"]
        total_cost = price_per_share * int(shares)

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        if cash < total_cost:
            return apology("not enough cash", 400)

        # Actualizar base de datos
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                   session["user_id"], symbol, int(shares), price_per_share)

        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total_cost, session["user_id"])

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute(
        "SELECT symbol, shares, price, transacted FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("Symbol required")

        stock = lookup(symbol)
        if stock is None:
            return apology("Symbol invalid", 400)

        return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # return apology("TODO")
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("Username required", 400)
        elif not password:
            return apology("Password required", 400)
        elif not confirmation:
            return apology("Password confirmation required", 400)
        elif password != confirmation:
            return apology("Password do not match", 400)

        try:
            hash_password = generate_password_hash(password)
            db.execute("insert into users (username, hash) values (?,?)", username, hash_password)
        except ValueError:
            return apology("Username alredy exists", 400)

        return render_template("login.html")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Verificar que el campo de acciones no esté vacío y que sea positivo
        if not symbol:
            return apology("must provide symbol", 400)
        if not shares.isdigit() or int(shares) <= 0:
            return apology("must provide positive number of shares", 400)

        # Verificar que el usuario posee suficientes acciones
        stock_owned = db.execute(
            "SELECT SUM(shares) AS stock_owned FROM transactions WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        print(f"stock_owned: {stock_owned}")

        if not stock_owned or stock_owned[0]["stock_owned"] < int(shares):
            return apology("not enough shares", 400)

        # Actualizar base de datos
        price_per_share = lookup(symbol)["price"]

        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?",
                   price_per_share * int(shares), session["user_id"])

        # Registrar la venta en la tabla de transacciones (venta = negativo)
        db.execute("""
            INSERT INTO transactions (user_id, symbol, shares, price)
            VALUES (?, ?, ?, ?)
        """, user_id, symbol, -int(shares), price_per_share)

        return redirect("/")
    else:
        stocks = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])
        return render_template("sell.html", stocks=stocks)
