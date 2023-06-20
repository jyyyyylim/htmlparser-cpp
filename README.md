# htmlparser-cpp
babby's first C++ project. *refactor in progress*

## raison d'etre
\- i needed parsing capability for another one of my projects </br>

## progress
- [x] Just Works
- [ ] 支持 CJK 字符集
- [x] given a html5-compliant file input, accurately produces a doubly linked general tree representation of the DOM
- [x] accurately preserves ALL tag attributes
- [x] shouldnt discriminate against even the most horrendously formatted markup
- [ ] any facility whatsoever to process the parsed tree
- [x] parses at reasonable speed
- [ ] support of emmet-like input rules to the parser


## limitations
\- a 1-week old cpp dev birthed this into existence. ~do point out any better approach to the spaghetti that is the parsing logic~ </br>
\- will break down at javascript embeds if the raw string `</script>` is involved. clueless as to how to deal with it at the moment </br> 
\- only built with trusted inputs in consideration. use recklessly at your own risk</br> 
\- no "parse exceptions" of any kind implemented</br>
\- does not preserve script and stylesheet code embeds</br>
\- excessive spaces within the content are not ignored, though its, at worst, an annoyance that doesnt affect the accuracy of the structure
