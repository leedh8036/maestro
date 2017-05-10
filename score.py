import MySQLdb, os, unittest
from midi_comparator import MIDIComparator

# connect db
db = MySQLdb.connect(host="35.161.154.86",
			user="root",
			passwd="dong8036",
			db="score",
			charset="utf8")

cur = db.cursor()

# folder stored user's midi file
path_dir='/home/yunju/midi3/midi_compare'

file_list=os.listdir(path_dir)
file_list.sort()

# search file
for item in file_list:
	if item.find('score2') is not -1:
		print item
		user_mid=item


class TestPitch(unittest.TestCase):          
	def test_one_note_diff_sequences(self):
		mc = MIDIComparator(user_mid, 'tests/data/score3.mid')
		# score3 is missing one note present in score 2
		ratio = 100 * mc.get_similarity_ratio()
		print 'score: %.f' % ratio 
		#assert 0.87 < ratio < 0.92
                

#--record score
#sql = "INSERT INTO SCHOOL(user,score) VALUE(%d, %d)"
#cur.execute("INSERT INTO SCHOOL VALUES(a,ratio)")

cur.execute("SELECT * FROM SCHOOL")

for row in cur.fetchall():
	print row[0], row[1]

db.commit()
db.close()



if __name__ == "__main__":
    unittest.main()





