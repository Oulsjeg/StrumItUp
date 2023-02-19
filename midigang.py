import mido
from pychord import find_chords_from_notes
import os
import serial
import struct

def get_chord(lst, note_name):
    '''Returns a list of chords that can be made from the given notes'''
    if(lst):
        names = []
        for n in lst:
            names.append(note_name[n])
        return find_chords_from_notes(names);
    return []

def to_ms(time, tempo, ticks_per_beat):
    '''Converts midi time to milliseconds'''
    return tempo * time / ticks_per_beat

def slash(string):
    '''Removes the slash from a chord name'''
    if len(string) > 2:
        idx = string.index('/')
        return string[:idx]
    if 'm' in string:
        return string[0]
    return string

def parse_midi(filename):
    '''Parses a midi file and returns a list of chords and their lengths'''
    # Open the midi file
    mid = mido.MidiFile('midi/' + filename)

    # Get the tempo and ticks per beat
    tempo = mid.tracks[0][0].tempo
    ticks_per_beat = mid.ticks_per_beat

    ## set up a bunch of stuff before parsing the midi file
    # Dictionary of note names and their midi values
    note_name = {127 : "G",
                126 : "F#",
                125 : "F",
                124 : "E",
                123 : "D#",
                122 : "D",
                121 : "C#",
                120 : "C",
                119 : "B",
                118 : "A#",
                117 : "A",
                116 : "G#",
                115 : "G",
                114 : "F#",
                113 : "F",
                112 : "E",
                111 : "D#",
                110 : "D",
                109 : "C#",
                108 : "C",
                107 : "B",
                106 : "A#",
                105 : "A",
                104 : "G#",
                103 : "G",
                102 : "F#",
                101 : "F",
                100 : "E",
                99 : "D#",
                98 : "D",
                97 : "C#",
                96 : "C",
                95 : "B",
                94 : "A#",
                93 : "A",
                92 : "G#",
                91 : "G",
                90 : "F#",
                89 : "F",
                88 : "E",
                87 : "D#",
                86 : "D",
                85 : "C#",
                84 : "C",
                83 : "B",
                82 : "A#",
                81 : "A",
                80 : "G#",
                79 : "G",
                78 : "F#",
                77 : "F",
                76 : "E",
                75 : "D#",
                74 : "D",
                73 : "C#",
                72 : "C",
                71 : "B",
                70 : "A#",
                69 : "A",
                68 : "G#",
                67 : "G",
                66 : "F#",
                65 : "F",
                64 : "E",
                63 : "D#",
                62 : "D",
                61 : "C#",
                60 : "C",
                59 : "B",
                58 : "A#",
                57 : "A",
                56 : "G#",
                55 : "G",
                54 : "F#",
                53 : "F",
                52 : "E",
                51 : "D#",
                50 : "D",
                49 : "C#",
                48 : "C",
                47 : "B",
                46 : "A#",
                45 : "A",
                44 : "G#",
                43 : "G",
                42 : "F#",
                41 : "F",
                40 : "E",
                39 : "D#",
                38 : "D",
                37 : "C#",
                36 : "C",
                35 : "B",
                34 : "A#",
                33 : "A",
                32 : "G#",
                31 : "G",
                30 : "F#",
                29 : "F",
                28 : "E",
                27 : "D#",
                26 : "D",
                25 : "C#",
                24 : "C",
                23 : "B",
                22 : "A#",
                21 : "A"}

    note_on = 'note_on'
    note_off = 'note_off'
    track = mid.tracks[1]
    pianoroll = []
    prev = []
    time = 0

    ## parse the midi file
    # loop through the midi file and add the notes to the pianoroll
    for msg in track:
        if(msg.type == note_on):
            if(msg.time != 0):
                time = msg.time
            prev.append(msg.note)
        if(msg.type == note_off):
            if(len(prev) > 0):
                pianoroll.append((get_chord(prev, note_name) , to_ms(time, tempo, ticks_per_beat)))
                prev=[]
            else:
                prev = []

    # store the chords and their lengths in a text file
    chords = ''
    lengths = []
    for i in pianoroll:
        # if there are multiple chords, just use the first one
        chords += slash(i[0][0].chord)
        lengths.append(int(i[1]))
        # print(i)
    chords += '\n'
    lengths.append('\n')

    # fname = filename.split('.')[0] # get the filename without the extension
    # with open(f'chords/{fname}_chords.txt', 'w') as f:
    #     # print the chords and their lengths to the text file
    #     print(chords, file=f)
    #     print(lengths, file=f)
    #     print(tempo, file=f)
    #     print(ticks_per_beat, file=f)
    return chords, lengths

def send_to_arduino(chords, durations):
    '''Sends the chords and their lengths to the arduino'''
    # try:
    # set up serial comms
    # ser = serial.Serial('COM5', 9600)

    #clear output buffer
    # ser.reset_output_buffer()

    # send chord data
    print('sending chords')
    ser.write(bytes(chords, 'utf-8'))
    # wait for all data to be sent

    while ser.out_waiting:
        pass
    print('Sent chords!')
    # wait for all data to be sent
    # print(ser.read())
    # send duration data
    for i in durations:
        # if i in ['[', ']', ',']:
        #     continue
        # i = i.replace(' ', '')
        # i = i.replace(']', '')
        # i = i.replace('[', '')
        # i = i.replace(',', '')
        # i = i.replace('\n', '')
        # i = float(i)
        # i = struct.pack('f', i)
        ser.write(bytes(i))

    print('sending durations')
    # wait for all data to be sent
    while ser.out_waiting:
        pass
    print('Sent durations!')

    # send a character to indicate end of duration data
    ser.write(bytes('\n', 'utf_8'))

    # close serial comms
    ser.close()

    print("Done")
    # except:
    #     print('Error sending data to arduino')

def parse_IR_data(number):
    '''Parses the IR data from the arduino'''
    if number == b'\x16':
        return 0
    elif number == b'\x0c':
        return 1
    elif number == b'\x18':
        return 2
    elif number == b'^':
        return 3
    elif number == b'\x08':
        return 4
    elif number == b'\x1c':
        return 5
    elif number == b'Z':
        return 6
    elif number == b'B':
        return 7
    elif number == b'R':
        return 8
    elif number == b'J':
        return 9
    elif number == b'E':
        return 'off'
    else:
        return 'unknown'

if __name__ == '__main__':
    # get the list of midi files
    song_list = os.listdir('midi')

    # while True:
    # set up serial comms with arduino to get data, clear input buffer
    ser = serial.Serial('COM5', 9600)
    # ser.reset_input_buffer()

    # wait for arduino to send something
    while not ser.in_waiting:
        pass
    
    # read the number from the arduino
    number = ser.read()
    filenumber = parse_IR_data(number)
    print(filenumber)

    if filenumber == 'off' or filenumber == 'unknown':
        # turn off
        ser.close()
    
    filename = song_list[filenumber%len(song_list)]

    # close serial comms
    # parse the midi file
    chords, lengths = parse_midi(filename)

    # fname = filename.split('.')[0] # get the filename without the extension
    # read the chords and their lengths from the text file
    # with open(f'chords/{fname}_chords.txt', 'r') as f:
    # chords = f.readline()
    # durations = f.readline()
    # tempo = f.readline()
    # ticks_per_beat = f.readline()

    # send the chords and their lengths to the arduino
    send_to_arduino(chords, lengths)