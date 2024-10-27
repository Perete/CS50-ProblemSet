-- In 13.sql, write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
-- Your query should output a table with a single column for the name of each person.
-- There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
-- Kevin Bacon himself should not be included in the resulting list.

SELECT DISTINCT p2.name
FROM people AS p1
JOIN stars AS s1 ON p1.id = s1.person_id
JOIN movies AS m ON s1.movie_id = m.id
JOIN stars AS s2 ON m.id = s2.movie_id
JOIN people AS p2 ON s2.person_id = p2.id
WHERE p1.name = 'Kevin Bacon' AND p1.birth = 1958
AND p2.name != 'Kevin Bacon';
