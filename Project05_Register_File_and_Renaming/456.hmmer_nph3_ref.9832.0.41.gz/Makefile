TOP_DIR = $(PWD)
SIMENV_SYSROOT = $(TOP_DIR)/simenv

APP_NAME = 456.hmmer_nph3_ref
APP_CMD = hmmer_base.rv64 nph3.hmm swiss41
APP_INIT_CWD = /app
APP_MEMSIZE = 256

SIM = ./721sim
SIM_FLAGS = -c/mnt/designkits/spec_2006_2017/O2_fno_bbreorder/app_storage/checkpoints/456.hmmer_nph3_ref/456.hmmer_nph3_ref.9832.0.41.gz
FESVR_FLAGS = 
PK_FLAGS = 

SIM_FLAGS_EXTRA =
FESVR_FLAGS_EXTRA =
PK_FLAGS_EXTRA =
APP_CMD_EXTRA =

.PHONY: cleanrun cleangdb envsetup envcheck envclean run gdb app_target_cwd app_target_cmd app_mem

cleanrun: envclean envsetup run
cleangdb: envclean envsetup gdb

envsetup:
	@ echo Setting up a new simenv at $(SIMENV_SYSROOT)
	atool-simenv spawn $(APP_NAME) $(SIMENV_SYSROOT)

envcheck:
	@ echo Verifing the simenv at $(SIMENV_SYSROOT)
	atool-simenv verify $(APP_NAME) $(SIMENV_SYSROOT)

envclean:
	@ echo Removing the simenv at $(SIMENV_SYSROOT)
	rm -frv $(SIMENV_SYSROOT)

run: envcheck
	@ echo Starting simulation
	$(SIM) -m$(APP_MEMSIZE) $(SIM_FLAGS) $(SIM_FLAGS_EXTRA) $(FESVR_FLAGS) $(FESVR_FLAGS_EXTRA) +chroot=$(SIMENV_SYSROOT) +target-cwd=$(APP_INIT_CWD) pk $(PK_FLAGS) $(PK_FLAGS_EXTRA) $(APP_CMD) $(APP_CMD_EXTRA)

gdb: envcheck
	@ echo Starting simulation in gdb
	gdb --args $(SIM) -m$(APP_MEMSIZE) $(SIM_FLAGS) $(SIM_FLAGS_EXTRA) $(FESVR_FLAGS) $(FESVR_FLAGS_EXTRA) +chroot=$(SIMENV_SYSROOT) +target-cwd=$(APP_INIT_CWD) pk $(PK_FLAGS) $(PK_FLAGS_EXTRA) $(APP_CMD) $(APP_CMD_EXTRA)

app_target_cwd:
	@ echo "$(APP_INIT_CWD)"

app_target_cmd:
	$(eval SIMENV_SYSROOT=)
	@ echo "$(APP_CMD)"

app_mem:
	@ echo "$(APP_MEMSIZE)"
