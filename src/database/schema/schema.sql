CREATE TABLE Loop (
    loop_id INTEGER PRIMARY KEY,
    loop_name TEXT,
    loop_length TEXT,
    loop_bpm INTEGER
);

CREATE TABLE Track (
    track_id INTEGER PRIMARY KEY,
    loop_id INTEGER,
    track_name TEXT,
    track_type TEXT,
    track_file_path TEXT,
    FOREIGN KEY (loop_id) REFERENCES Loop(loop_id)
)