# htmlparser-cpp
babby's first html parser in C++, overengineered

## raison d'etre
\- i needed parsing capability for another one of my projects </br>

## aims
- [x] Just Works
- [x] given a html5-compliant file input, accurately produces a doubly linked general tree
- [x] accurately preserves ALL tag attributes
- [x] shouldnt discriminate against even the most horrendously formatted markup
- [ ] any facility whatsoever to process the parsed tree
- [x] parses at reasonable speed
- [ ] support of emmet-like input rules to the parser


## limitations
\- a 1-week old cpp dev birthed this into existence. do point out any better approach to the spaghetti that is the parsing logic </br>
\- the 1-week old baby in question got brain cancer halfway through this project </br>
\- will break down at javascript embeds if the raw string `</script>` is involved. no idea how to deal with it at the moment </br> 
\- only built for trusted page input. use recklessly at your own risk</br> 
\- no "parse exceptions" of any kind implemented</br>
\- does not preserve script and stylesheet code embeds</br>
\- excessive spaces within the content are not ignored, though its at worst an annoyance that doesnt affect the accuracy of the structure
