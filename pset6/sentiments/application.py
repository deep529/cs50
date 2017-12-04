from flask import Flask, redirect, render_template, request, url_for
import os
import sys
import helpers
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    screen_name = screen_name.strip('@')
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name)
        
    # absolute path to list
    positive = os.path.join(sys.path[0], "positive-word.txt")
    negative = os.path.join(sys.path[0], "negative-word.txt")
    
    # instantiate analyzer
    analyzer = Analyzer(positive, negative)
    
    pos, neg, neutr = 0.0, 0.0, 100.0
    
    if tweets==None:
        pos = 1
        neg = 1
        neutr = 1
    else:        
        # calculations
        for tweet in tweets:
            score = analyzer.analyze(tweet)
            
            if score>0.0:
                pos = pos + 1
            elif score<0.0:
                neg = neg + 1
            else:
                neutr = neutr + 1
            
    # generate chart
    chart = helpers.chart(pos, neg, neutr)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)