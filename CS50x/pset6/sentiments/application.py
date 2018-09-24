from flask import Flask, redirect, render_template, request, url_for

import helpers
import os
import sys
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, 100)

    # Return to index if unable to get tweets
    positive, negative, neutral = 0.0, 0.0, 0.0 
    if tweets == None:
        return redirect(url_for("index"))
    
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    
    # Load list of words, iterate over each tweet and analyze it, returning overall score
    analyzer = Analyzer(positives, negatives)
    
    for tweet in tweets:
        score = 0
        score = analyzer.analyze(tweet)
        if score > 0:
            positive += 1
        if score < 0:
            negative += 1
        if score == 0:
            neutral += 1
    
    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
