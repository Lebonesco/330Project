CREATE TABLE metafiles (
	file_pk serial primary key,
	metafile varchar(122),
	peers text[]
);
