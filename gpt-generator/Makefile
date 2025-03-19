run:
	@-rm gpt.img
	gcc -Wall -o gpt_generator.elf gpt_generator.c crc.c gpt.c
	./gpt_generator.elf
	chmod u+r gpt.img
#	cp gpt.img ../platform/