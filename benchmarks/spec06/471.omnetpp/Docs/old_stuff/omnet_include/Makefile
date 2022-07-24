#
# Definitions to be customized by 'configure'
#
OMNETPP_DOC_DIR=/spec/omnet++/omnetpp-2.3/doc
DOXYGEN=not found

all: doc

doc:
	# NOTE: doxy.cfg has the output directory hardcoded!
	$(DOXYGEN) doxy.cfg

clean:
	- rm $(OMNETPP_DOC_DIR)/api/*
