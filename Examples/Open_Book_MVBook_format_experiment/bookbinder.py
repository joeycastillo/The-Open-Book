import binascii
import struct
import textwrap
import hackdown2

class LineInfo:
    def __init__(self, offset, length):
        self.offset = offset
        self.length = length

class Book:
    def __init__(self, markdown):
        self.title = markdown.metadata['title']
        self.author = markdown.metadata['author']
        self.text = markdown
    
    def generate_obk(self):
        data = bytearray()
        page_lut = list()
        magic = bytearray('OPENBOOK', encoding='utf8')
        title = bytearray(self.title, encoding='utf8')
        author = bytearray(self.author, encoding='utf8')
        text = bytearray()
        page_lut.append(LineInfo(0, len(title))) # first 'page' contains title
        page_lut.append(LineInfo(page_lut[-1].offset + page_lut[-1].length, len(author))) # second 'page' contains author
        paragraphs = self.text.split('\n')

        heights = list()
        # this is just to trick the word wrapper into keeping our blank lines
        for i in range(0, len(paragraphs)):
            heights.append(1)
            if paragraphs[i] == '':
                paragraphs[i] = '|'

        # wrap header lines with larger text size
        for i in range(0, len(paragraphs)):
            if paragraphs[i].startswith('\uFDDE'):
                wrapped = None
                height = None
                if paragraphs[i][1] == '4':
                    wrapped = textwrap.wrap(paragraphs[i], 6)
                    height = 4
                elif paragraphs[i][1] == '3':
                    wrapped = textwrap.wrap(paragraphs[i], 12)
                    height = 3
                elif paragraphs[i][1] == '2':
                    wrapped = textwrap.wrap(paragraphs[i], 17)
                    height = 2
                if wrapped is not None:
                    del paragraphs[i]
                    del heights[i]
                    paragraphs.insert(i, '|')
                    heights.insert(i, 1)
                    wrapped.reverse()
                    for line in wrapped:
                        paragraphs.insert(i, line)
                        heights.insert(i, height)
        current_line = 0
        current_length = 0
        for i in range(0, len(paragraphs)):
            lines = textwrap.wrap(paragraphs[i], 32)
            for line in lines:
                if current_line + heights[i] > 22:
                    page_lut.append(LineInfo(page_lut[-1].offset + page_lut[-1].length, current_length))
                    current_line = 0
                    current_length = 0
                if line == '|':
                    line_bytes = bytearray('\n', encoding='utf8')
                else:
                    line_bytes = bytearray(line + '\n', encoding='utf8')
                current_length += len(line_bytes)
                current_line += heights[i]
                text += line_bytes

        page_lut[-1].length |= 0x8000 # end of text marker
        start_of_data = len(magic) + len(page_lut) * 12
        data += magic
        for entry in page_lut:
            data += struct.pack('<Q', entry.offset + start_of_data)
            data += struct.pack('<L', entry.length)
        data += title
        data += author
        data += text
        return data
        
while 1:
    print('\033[;1mMinimum Viable Book Binder\033[0;0m\n▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔')
    filename = input('Input text file (without extension): ')
    text = None
    with open(filename + '.txt', 'r') as infile:
        text = infile.read()
    markdown = hackdown2.markdown(text, extras=['metadata'])
    meta = markdown.metadata
    book = Book(markdown)
    output = book.generate_obk()
    print('Generated Open Book data of length', len(output))
    outfile = open(filename + '.obk', 'wb')
    outfile.write(output)
    outfile.close()