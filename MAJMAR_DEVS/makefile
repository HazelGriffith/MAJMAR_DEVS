CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I../cadmium

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

evacInfo.o: data_structures/evacInfo.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) data_structures/evacInfo.cpp -o build/evacInfo.o
	
heloInfo.o: data_structures/heloInfo.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) data_structures/heloInfo.cpp -o build/heloInfo.o

main_filter_loc_test.o: tests/testing_mains/main_filter_loc_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_filter_loc_test.cpp -o build/main_filter_loc_test.o
	
main_filter_evac_test.o: tests/testing_mains/main_filter_evac_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_filter_evac_test.cpp -o build/main_filter_evac_test.o
	
main_filter_es_test.o: tests/testing_mains/main_filter_es_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_filter_es_test.cpp -o build/main_filter_es_test.o
	
main_evacuee_manager_test.o: tests/testing_mains/main_evacuee_manager_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_evacuee_manager_test.cpp -o build/main_evacuee_manager_test.o
	
main_helicopter_test.o: tests/testing_mains/main_helicopter_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_helicopter_test.cpp -o build/main_helicopter_test.o
	
main_evacuee_wc_test.o: tests/testing_mains/main_evacuee_wc_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_evacuee_wc_test.cpp -o build/main_evacuee_wc_test.o

main_evacuee_rc_test.o: tests/testing_mains/main_evacuee_rc_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_evacuee_rc_test.cpp -o build/main_evacuee_rc_test.o
	
main_helipad_manager_test.o: tests/testing_mains/main_helipad_manager_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_helipad_manager_test.cpp -o build/main_helipad_manager_test.o
	
main_helicopter_queue_test.o: tests/testing_mains/main_helicopter_queue_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_helicopter_queue_test.cpp -o build/main_helicopter_queue_test.o

main_fol_test.o: tests/testing_mains/main_fol_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_fol_test.cpp -o build/main_fol_test.o
	
main_coast_guard_ship_test.o: tests/testing_mains/main_coast_guard_ship_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_coast_guard_ship_test.cpp -o build/main_coast_guard_ship_test.o
	
main_helicopter_coupled_test.o: tests/testing_mains/main_helicopter_coupled_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_helicopter_coupled_test.cpp -o build/main_helicopter_coupled_test.o
	
main_evacuee_coupled_test.o: tests/testing_mains/main_evacuee_coupled_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_evacuee_coupled_test.cpp -o build/main_evacuee_coupled_test.o
	
main_evacuation_site_test.o: tests/testing_mains/main_evacuation_site_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) tests/testing_mains/main_evacuation_site_test.cpp -o build/main_evacuation_site_test.o
	
main.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) top_model/main.cpp -o build/main.o
	
#TARGET FOR TESTS ONLY
tests: main_filter_loc_test.o main_filter_evac_test.o main_filter_es_test.o main_evacuee_manager_test.o main_helicopter_test.o main_evacuee_wc_test.o main_evacuee_rc_test.o main_helipad_manager_test.o main_helicopter_queue_test.o main_fol_test.o main_coast_guard_ship_test.o main_helicopter_coupled_test.o main_evacuee_coupled_test.o main_evacuation_site_test.o evacInfo.o heloInfo.o
		$(CC) -g -o bin/FILTER_LOC_TEST build/main_filter_loc_test.o build/evacInfo.o
		$(CC) -g -o bin/FILTER_EVAC_TEST build/main_filter_evac_test.o build/evacInfo.o
		$(CC) -g -o bin/FILTER_ES_TEST build/main_filter_es_test.o build/heloInfo.o
		$(CC) -g -o bin/EVACUEE_MANAGER_TEST build/main_evacuee_manager_test.o build/heloInfo.o build/evacInfo.o
		$(CC) -g -o bin/HELICOPTER_TEST build/main_helicopter_test.o build/heloInfo.o build/evacInfo.o
		$(CC) -g -o bin/EVACUEE_WC_TEST build/main_evacuee_wc_test.o build/evacInfo.o
		$(CC) -g -o bin/EVACUEE_RC_TEST build/main_evacuee_rc_test.o build/evacInfo.o
		$(CC) -g -o bin/HELIPAD_MANAGER_TEST build/main_helipad_manager_test.o build/heloInfo.o
		$(CC) -g -o bin/HELO_QUEUE_TEST build/main_helicopter_queue_test.o build/heloInfo.o
		$(CC) -g -o bin/FOL_TEST build/main_fol_test.o build/evacInfo.o
		$(CC) -g -o bin/COAST_GUARD_SHIP_TEST build/main_coast_guard_ship_test.o
		$(CC) -g -o bin/HELO_COUP_TEST build/main_helicopter_coupled_test.o build/heloInfo.o build/evacInfo.o
		$(CC) -g -o bin/EVAC_COUP_TEST build/main_evacuee_coupled_test.o build/evacInfo.o
		$(CC) -g -o bin/EVAC_SITE_TEST build/main_evacuation_site_test.o build/heloInfo.o build/evacInfo.o
		
evacManTest: main_evacuee_manager_test.o evacInfo.o heloInfo.o
		$(CC) -g -o bin/EVACUEE_MANAGER_TEST build/main_evacuee_manager_test.o build/heloInfo.o build/evacInfo.o
		
helicopterTest: main_helicopter_test.o evacInfo.o heloInfo.o
		$(CC) -g -o bin/HELICOPTER_TEST build/main_helicopter_test.o build/heloInfo.o build/evacInfo.o
		
evacueeWcTest: main_evacuee_wc_test.o evacInfo.o
		$(CC) -g -o bin/EVACUEE_WC_TEST build/main_evacuee_wc_test.o build/evacInfo.o
		
evacueeRcTest: main_evacuee_rc_test.o evacInfo.o
		$(CC) -g -o bin/EVACUEE_RC_TEST build/main_evacuee_rc_test.o build/evacInfo.o
		
helipadManTest: main_helipad_manager_test.o heloInfo.o
		$(CC) -g -o bin/HELIPAD_MANAGER_TEST build/main_helipad_manager_test.o build/heloInfo.o
		
heloQueueTest: main_helicopter_queue_test.o heloInfo.o evacInfo.o
		$(CC) -g -o bin/HELO_QUEUE_TEST build/main_helicopter_queue_test.o build/heloInfo.o
		
evacSiteTest: main_evacuation_site_test.o heloInfo.o evacInfo.o
		$(CC) -g -o bin/EVAC_SITE_TEST build/main_evacuation_site_test.o build/heloInfo.o build/evacInfo.o
		
simulator: main.o evacInfo.o heloInfo.o
		$(CC) -g -o bin/MAIN build/main.o build/heloInfo.o build/evacInfo.o

.PHONY: clean
#CLEAN COMMANDS
clean: 
	rm -f bin/* build/*