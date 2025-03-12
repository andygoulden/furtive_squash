.DEFAULT_GOAL:=furtive_squash

.PHONY: furtive_squash
furtive_squash:
	@make -C third_party -f stm32f7_hal.mk
	@make -C third_party -f freertos.mk
	@make -C third_party -f cmsis_f7.mk
	@make -C third_party -f sx126x.mk
	@make -C src/app -f furtive_squash.mk

.PHONY: clean
clean:
	@make -C src/app -f furtive_squash.mk clean

.PHONY: clean_all
clean_all:
	@make -C third_party -f stm32f7_hal.mk clean
	@make -C third_party -f freertos.mk clean
	@make -C third_party -f cmsis_f7.mk clean
	@make -C third_party -f sx126x.mk
	@make -C src/app -f furtive_squash.mk clean

.PHONY: flash
flash:
	@make -C third_party -f stm32f7_hal.mk
	@make -C third_party -f freertos.mk
	@make -C third_party -f cmsis_f7.mk
	@make -C third_party -f sx126x.mk
	@make -C src/app -f furtive_squash.mk
	@make -C src/app -f furtive_squash.mk flash

.PHONY: serial
serial:
	@sudo screen /dev/ttyACM0 115200
