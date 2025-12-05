#!/usr/bin/python3

import csv
import sqlite3
import sys

def main(fname: str):
    with open(fname, 'r', encoding='utf-8') as fin:
        reader = csv.DictReader(fin)
        
        with sqlite3.connect("movies.s3db") as conn:
            cur = conn.cursor()
            
            if 'movie' in fname.lower():
                for row in reader:
                    cur.execute(
                        "INSERT OR IGNORE INTO movies (movieId, title, genres) VALUES (?, ?, ?)",
                        (int(row['movieId']), row['title'], row['genres'])
                    )
            elif 'rating' in fname.lower():
                for row in reader:
                    cur.execute(
                        "INSERT INTO ratings (userId, movieId, rating, timestamp) VALUES (?, ?, ?, ?)",
                        (int(row['userId']), int(row['movieId']), float(row['rating']), int(row['timestamp']))
                    )
            
            conn.commit()

if __name__ == "__main__":
    main(sys.argv[1])