import MySQLdb, os, sys
from midi_comparator import MIDIComparator

user_id = sys.argv[1]
song_name = sys.argv[2]


# connect db
db = MySQLdb.connect(host="35.161.154.86",
			user="root",
			passwd="dong8036",
			db="score",
			charset="utf8")

cur = db.cursor()

# folder stored user's midi file
path_dir='tests/data'

file_list=os.listdir(path_dir)
file_list.sort()

# search file
for item in file_list:
	if item.find('score2') is not -1:
		print item
		user_mid=item

          
def test_one_note_diff_sequences():
	mc = MIDIComparator(user_mid, 'tests/data/score3.mid')
	# score3 is missing one note present in score 2
	ratio = 100 * mc.get_similarity_ratio()
	print 'score: %.f' % ratio 
	return ratio
	#assert 0.87 < ratio < 0.92
                
ratio = test_one_note_diff_sequences()

#--record score
insert_sql = "INSERT INTO SCORE(user,song,score) VALUE(%s, %s, %s)"
cur.execute(insert_sql, (user_id, song_name, ratio))

cur.execute("SELECT * FROM SCORE")

for row in cur.fetchall():
	print row[0], row[1], row[2]

db.commit()
db.close()







