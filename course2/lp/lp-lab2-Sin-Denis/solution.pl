% Author: student Sin Denis
% Group: M80-207B-18
% Lab02 Logical programming

lang(english).
lang(french).
lang(german).
lang(italian).

delegat(Lang1, Lang2).

speak(delegat(Lang, _), Lang).
speak(delegat(_, Lang), Lang).

speak_common_lang([D1, D2, D3, D4]) :-
	speak(D1, Lang),
	speak(D2, Lang),
	speak(D3, Lang),
	speak(D4, Lang).

three_speak_lang([D1, D2, D3, D4]) :-
	speak(D1, Lang), speak(D2, Lang), speak(D3, Lang);
	speak(D1, Lang), speak(D2, Lang), speak(D4, Lang);
	speak(D1, Lang), speak(D3, Lang), speak(D4, Lang);
	speak(D2, Lang), speak(D3, Lang), speak(D4, Lang).
	

not_speak_french_german([D1, D2, D3, D4]) :-
	(not(speak(D1, french)); not(speak(D1, german))),
	(not(speak(D2, french)); not(speak(D2, german))),
	(not(speak(D3, french)); not(speak(D3, german))),
	(not(speak(D4, french)); not(speak(D4, german))).

fizik_matematik_himik_speak([D1, D2, D3, D4]) :-
	speak(D1, Lang), speak(D3, Lang), speak(D4, Lang).

before(Lang1, Lang2, [Lang1|Rest]) :-
	member(Lang2, Rest).
before(Lang1, Lang2, [X|Rest]) :-
	before(Lang1, Lang2, Rest).

langs([english, french, german, italian]).

solve(Solve) :-
	langs(Langs),
	Solve = [Fizik, Biolog, Matematik, Himik],
	lang(ALang1), lang(ALang2), before(ALang1, ALang2, Langs),
	lang(BLang1), lang(BLang2), before(BLang1, BLang2, Langs),
	lang(CLang1), lang(CLang2), before(CLang1, CLang2, Langs),
	lang(DLang1), lang(DLang2), before(DLang1, DLang2, Langs),
	Fizik = delegat(ALang1, ALang2),
	Biolog = delegat(BLang1, BLang2),
	Matematik = delegat(CLang1, CLang2),
	Himik = delegat(DLang1, DLang2),
	not(speak_common_lang(Solve)),
	three_speak_lang(Solve),
	not_speak_french_german(Solve),
	not(speak(Fizik, english)),
	speak(Fizik, T), speak(Matematik, T), speak(Fizik, K), speak(Biolog, K),
	speak(Biolog, german),
	speak(Biolog, L), speak(Himik, L),
	not(speak(Himik, german)),
	not(fizik_matematik_himik_speak(Solve)).	

get_all_solves(Solves) :-
	setof(Solve, solve(Solve), Solves).

print_ans([delegat(Al1, Al2), delegat(Bl1, Bl2), delegat(Cl1, Cl2), delegat(Dl1, Dl2)]) :-
	write('Fizik speak '), write(Al1), write(' and '), write(Al2), nl,
	write('Biolog speak '), write(Bl1), write(' and '), write(Bl2), nl,
	write('Matematik speak '), write(Cl1), write(' and '), write(Cl2), nl,
	write('Himik speak '), write(Dl1), write(' and '), write(Dl2), nl.

print_list([Elem]) :-
	print_ans(Elem), nl.
print_list([Head|Rest]) :-
	print_ans(Head), nl,
	print_list(Rest).

print_solve() :-
	get_all_solves(Solves),
	print_list(Solves).
