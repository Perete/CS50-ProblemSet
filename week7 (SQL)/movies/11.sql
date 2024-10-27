-- In 11.sql, write a SQL query to list the titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated.
-- Your query should output a table with a single column for the title of each movie.
-- You may assume that there is only one person in the database with the name Chadwick Boseman.

select movies.title
from people
join stars on people.id = stars.person_id
join movies on stars.movie_id = movies.id
join ratings on movies.id = ratings.movie_id
where people.name = 'Chadwick Boseman'
order by ratings.rating DESC
limit 5;
