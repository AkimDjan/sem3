SELECT name
FROM Passenger
GROUP BY name
HAVING COUNT(*) > 1

SELECT name 
FROM Passenger
WHERE name IN (SELECT name FROM Passenger GROUP BY name HAVING COUNT(*)>1)

SELECT author, 
       MIN(price) AS min_price, 
       MAX(price) AS max_price
FROM books
WHERE author IN (
    SELECT author
    FROM books
    GROUP BY author
    HAVING SUM(price * amount) > 5000
)
GROUP BY author
ORDER BY min_price DESC

SELECT ROUND(AVG(price), 2) AS Средняя_цена,
       ROUND(SUM(price * amount), 2) AS Стоимость
FROM books
WHERE amount BETWEEN 5 AND 14