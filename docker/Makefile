MD := mkdir
DOCKER := docker

CURR_DIR=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

RELEASE_DIR=$(CURR_DIR)/releases
SRC_DIR=$(CURR_DIR)/../
CONF_DIR=$(CURR_DIR)/conf
IMG_DIR=$(CURR_DIR)/img/
EXECUTABLE_FOR_DOCKER=SecureDialogues-ubuntu-18.04-x86_64

DOCKERS=centos7 ubuntu-bionic ubuntu-xenial

$(shell   mkdir -p $(IMG_DIR))
$(shell   mkdir -p $(RELEASE_DIR))


.SECONDARY: $(addprefix $(IMG_DIR),$(addsuffix .image,$(DOCKERS)) )

all: $(DOCKERS)

%: $(addprefix $(IMG_DIR), %.image)
	$(DOCKER) run  --mount type=bind,source=$(RELEASE_DIR),target=/app/res --mount type=bind,source=$(SRC_DIR),target=/app/src secure-dialogues-$(strip $(subst .image,, $(notdir $<) ))

$(addprefix $(IMG_DIR), %.image): %.docker
	$(DOCKER) build -t  secure-dialogues-$(strip $(subst .docker,, $< )) --file $< . 
	$(file > $@, "ON")
	
docker:  $(RELEASE_DIR)/$(EXECUTABLE_FOR_DOCKER) $(addprefix $(IMG_DIR), docker.image)
	$(MD) -p $(CONF_DIR)
	$(DOCKER) run --mount type=bind,source=$(CONF_DIR),target=/app/conf -e DISPLAY=unix$$DISPLAY -e EXEC=releases/$(EXECUTABLE_FOR_DOCKER) -v /tmp/.X11-unix:/tmp/.X11-unix secure-dialogues-docker		
	
$(RELEASE_DIR)/$(EXECUTABLE_FOR_DOCKER): $(addprefix $(IMG_DIR), ubuntu-bionic.image)
	$(DOCKER) run  --mount type=bind,source=$(RELEASE_DIR),target=/app/res --mount type=bind,source=$(SRC_DIR),target=/app/src secure-dialogues-$(strip $(subst .image,, $(notdir $<) ))
	
	
clean:
	rm -Rf $(IMG_DIR)
	rm -Rf $(RELEASE_DIR)
	rm -Rf $(CONF_DIR)

