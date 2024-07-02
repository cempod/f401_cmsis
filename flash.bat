openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/f401_cmsis.elf verify reset exit"
