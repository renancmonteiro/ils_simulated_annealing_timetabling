all: ILS Model Restricoes BuscaLocal bin/Movimento.h.gch
	g++ -g -O0 -o ils bin/*.o -lm -Wall
	
ILS: bin/GeraSolucaoInicialGulosa.o bin/Ils.o bin/TimeTabling.o bin/Pertubacao.o
bin/GeraSolucaoInicialGulosa.o: GeraSolucaoInicialGulosa.cpp
	g++ -Wall -g -c GeraSolucaoInicialGulosa.cpp -o bin/GeraSolucaoInicialGulosa.o	
bin/Pertubacao.o: Pertubacao.cpp
	g++ -Wall -g -c Pertubacao.cpp -o bin/Pertubacao.o
bin/Ils.o: Ils.cpp
	g++ -Wall -g -c Ils.cpp -o bin/Ils.o
bin/TimeTabling.o: TimeTabling.cpp
	g++ -Wall -g -c TimeTabling.cpp -o bin/TimeTabling.o
	
Model: bin/Alocacao.o bin/Individuo.o bin/Disciplina.o bin/Professor.o bin/Restricao.h.gch bin/Timeslot.o bin/Aula.o bin/Curriculo.o bin/Problema.o bin/Sala.o
bin/Alocacao.o: Model/Alocacao.cpp 
	g++ -Wall -g -c Model/Alocacao.cpp -o bin/Alocacao.o
bin/Individuo.o: Model/Individuo.cpp 
	g++ -Wall -g -c Model/Individuo.cpp -o bin/Individuo.o
bin/Disciplina.o: Model/Disciplina.cpp 
	g++ -Wall -g -c Model/Disciplina.cpp -o bin/Disciplina.o
bin/Professor.o: Model/Professor.cpp 
	g++ -Wall -g -c Model/Professor.cpp -o bin/Professor.o
bin/Restricao.h.gch: Restricoes/Restricao.h 
	g++ -Wall -g -c Restricoes/Restricao.h -o bin/Restricao.h.gch
bin/Timeslot.o: Model/Timeslot.cpp 
	g++ -Wall -g -c Model/Timeslot.cpp -o bin/Timeslot.o
bin/Aula.o: Model/Aula.cpp
	g++ -Wall -g -c Model/Aula.cpp -o bin/Aula.o
bin/Curriculo.o: Model/Curriculo.cpp
	g++ -Wall -g -c Model/Curriculo.cpp -o bin/Curriculo.o
bin/Problema.o: Model/Problema.cpp
	g++ -Wall -g -c Model/Problema.cpp -o bin/Problema.o
bin/Sala.o: Model/Sala.cpp
	g++ -Wall -g -c Model/Sala.cpp -o bin/Sala.o
	
Restricoes: bin/RestricaoForte3Indisponibilidade.o bin/RestricaoForte4AulasConflitantes.o bin/RestricaoFraca1RoomCapacity.h.gch bin/RestricaoFraca2MinimumWorkingDays.o bin/RestricaoFraca3CurriculumCompactness.o bin/RestricaoFraca4RoomStability.o
bin/RestricaoForte3Indisponibilidade.o: Restricoes/RestricaoForte3Indisponibilidade.cpp 
	g++ -Wall -g -c Restricoes/RestricaoForte3Indisponibilidade.cpp -o bin/RestricaoForte3Indisponibilidade.o
bin/RestricaoForte4AulasConflitantes.o: Restricoes/RestricaoForte4AulasConflitantes.cpp
	g++ -Wall -g -c Restricoes/RestricaoForte4AulasConflitantes.cpp -o bin/RestricaoForte4AulasConflitantes.o
bin/RestricaoFraca1RoomCapacity.h.gch: Restricoes/RestricaoFraca1RoomCapacity.h
	g++ -Wall -g -c Restricoes/RestricaoFraca1RoomCapacity.h -o bin/RestricaoFraca1RoomCapacity.h.gch
bin/RestricaoFraca2MinimumWorkingDays.o: Restricoes/RestricaoFraca2MinimumWorkingDays.cpp
	g++ -Wall -g -c Restricoes/RestricaoFraca2MinimumWorkingDays.cpp -o bin/RestricaoFraca2MinimumWorkingDays.o
bin/RestricaoFraca3CurriculumCompactness.o: Restricoes/RestricaoFraca3CurriculumCompactness.cpp
	g++ -Wall -g -c Restricoes/RestricaoFraca3CurriculumCompactness.cpp -o bin/RestricaoFraca3CurriculumCompactness.o
bin/RestricaoFraca4RoomStability.o: Restricoes/RestricaoFraca4RoomStability.cpp
	g++ -Wall -g -c Restricoes/RestricaoFraca4RoomStability.cpp -o bin/RestricaoFraca4RoomStability.o
		
bin/Movimento.h.gch: Vizinhancas/Movimento.h Vizinhancas/Move.cpp Vizinhancas/Swap.cpp Vizinhancas/CadeiadeKempeExtendido.cpp
	g++ -Wall -g -c Vizinhancas/Move.cpp -o bin/Move.o
	g++ -Wall -g -c Vizinhancas/Swap.cpp -o bin/Swap.o
	g++ -Wall -g -c Vizinhancas/CadeiadeKempeExtendido.cpp -o bin/CadeiadeKempeExtendido.o
	g++ -Wall -g -c Vizinhancas/RoomMove.cpp -o bin/RoomMove.o
	g++ -Wall -g -c Vizinhancas/RoomMove2.cpp -o bin/RoomMove2.o
	g++ -Wall -g -c Vizinhancas/TimeMove.cpp -o bin/TimeMove.o
	g++ -Wall -g -c Vizinhancas/TimeMove2.cpp -o bin/TimeMove2.o

BuscaLocal: bin/BuscaLocal.h.gch bin/SimulatedAnnealing.o
bin/BuscaLocal.h.gch: BuscaLocal/BuscaLocal.h
	g++ -Wall -g -c BuscaLocal/BuscaLocal.h -o bin/BuscaLocal.h.gch
bin/SimulatedAnnealing.o: BuscaLocal/SimulatedAnnealing.cpp
	g++ -Wall -g -c BuscaLocal/SimulatedAnnealing.cpp -o bin/SimulatedAnnealing.o

clean:
	rm bin/*
	
