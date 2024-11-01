-- In 10.sql, write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.
-- Your query should output a table with a single column for the name of each person.
-- If a person directed more than one movie that received a rating of at least 9.0, they should only appear in your results once.

select people.name
from people
join directors on people.id = directors.person_id
join movies on directors.movie_id = movies.id
join ratings on movies.id = ratings.movie_id
where ratings.rating >= 9.0;
