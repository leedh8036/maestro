from mido import MidiFile
from difflib import SequenceMatcher

class PitchHandler(object):

    def get_pitch_sequence(self, track):
        result = ""
        for message in track:
            try:
                result += "%s " % message.note
            except AttributeError:
                pass
        return result


class RhythmHandler(object):

    def get_rhythm_sequence(self, track):
        result = []
        for message in track:
            if not hasattr(message, 'note'):
                continue
            try:
                result.append(message.time)
            except AttributeError:
                pass
        return result


class VelocityHandler(object):

    def get_velocity_sequence(self, track):
        result = []
        for message in track:
            if not hasattr(message, 'note'):
                continue
            try:
                if message.velocity != 0:
                    result.append(message.velocity)
            except AttributeError:
                pass
        return result

     
class MIDIComparator(PitchHandler, RhythmHandler, VelocityHandler):

    def __init__(self, file1, file2):
        self.file1 = file1
        self.file2 = file2
        self.mid1 = MidiFile(self.file1)
        self.mid2 = MidiFile(self.file2)

    def get_euclidean_distance(self, lista, listb):
        """Calculate euclidean distance between two sets of integers"""
        return sum( (b - a) ** 2 for a,b in zip(lista, listb) ) ** .5

    def get_pitch_similarity_ratio(self):
        """
        Compare pitch sequences in the two files and return sequence similarity ratio
        """
        track1 = self.mid1.tracks[0]
        track2 = self.mid2.tracks[0]
        seq1 = self.get_pitch_sequence(track1)
        seq2 = self.get_pitch_sequence(track2)
        matcher = SequenceMatcher(None, seq1, seq2)
        return matcher.ratio()

    def get_rhythm_similarity_ratio(self):
        """
        Compare note duration sequences in two files and return sequence similarity ratio
        """
        track1 = self.mid1.tracks[0]
        track2 = self.mid2.tracks[0]
        seq1 = self.get_rhythm_sequence(track1)
        seq2 = self.get_rhythm_sequence(track2)

        # remove pauses at the beginning:
        if seq1:
            seq1[0] = 0
        if seq2:
            seq2[0] = 0
            
        len_diff = abs(len(seq1) - len(seq2))
        distance = self.get_euclidean_distance(seq1, seq2)
        ratio = 1.0 - distance / sum(seq1 + seq2)  - len_diff / 100.0
        return ratio

    def get_velocity_similarity_ratio(self):
        """
        Compare note duration sequences in two files and return sequence similarity ratio
        """
        track1 = self.mid1.tracks[0]
        track2 = self.mid2.tracks[0]
        seq1 = self.get_velocity_sequence(track1)
        seq2 = self.get_velocity_sequence(track2)
        len_diff = abs(len(seq1) - len(seq2))
        distance = self.get_euclidean_distance(seq1, seq2)
        ratio =  1.0 - distance / sum(seq1 + seq2)  - len_diff / 100.0
        return ratio
            
    def get_similarity_ratio(self):
        rhythm = self.get_rhythm_similarity_ratio()
        pitch = self.get_pitch_similarity_ratio()
        velocity = self.get_velocity_similarity_ratio()
	#all_ratio = 100 * rhythm * pitch * velocity
	#return int(round(float(all_ratio))) 
        return 1.0 * rhythm * pitch * velocity
