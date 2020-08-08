sex('Sin Dmitry', m).
sex('Lim Olga', f).
sex('Sin Denis', m).
sex('I Ok Suni', f).
sex('Lim Ten Chury', m).
sex('Styazhkina Elena', f).
sex('Sin U Sik', m).
sex('Lim Tatiana', f).
sex('Lim Nikolay', m).
sex('Sin Natalia', f).
sex('Sin Boris', m).
sex('Li Maksim', m).
sex('Sin Tok Un', m).
sex('Sin Ok Suni', f).
sex('Sin Men Sik', m).
sex('Kim Antonina', f).
sex('Sin Viktor', m).
sex('Styazhkina Nina', f).
sex('Styazhkin Sergey', m).
sex('Li Dmitriy', m).
sex('Li Kiril', m).
sex('Lim Oksana', f).
sex('Lim Kiril', m).
sex('Lim Roman', m).
sex('Lim Nikolay jr', m).
sex('Sin Aleksandr', m).
sex('Sin Albert', m).
sex('Kim Elena', f).
sex('Lim En Que', m).
sex('Tsoi Ki Suki', f).
sex('Lim En Se', m).
sex('Lim Ten Sun', f).
sex('Kim Viktor', m).
sex('Kim Galina', f).
sex('I Sen Do', m).
sex('Che Ki Suki', f).
sex('I Su Boki', m).
sex('I Mikhail', m).
sex('I Aleksandr', m).
sex('I Sergey', m).
parent('Sin Dmitry', 'Sin Denis').
parent('Lim Olga', 'Sin Denis').
parent('Sin Dmitry', 'Sin Boris').
parent('Lim Olga', 'Sin Boris').
parent('Lim Ten Chury', 'Lim Olga').
parent('I Ok Suni', 'Lim Olga').
parent('Lim Ten Chury', 'Lim Tatiana').
parent('I Ok Suni', 'Lim Tatiana').
parent('Lim Ten Chury', 'Lim Nikolay').
parent('I Ok Suni', 'Lim Nikolay').
parent('Sin U Sik', 'Sin Dmitry').
parent('Styazhkina Elena', 'Sin Dmitry').
parent('Sin U Sik', 'Sin Natalia').
parent('Styazhkina Elena', 'Sin Natalia').
parent('Li Dmitriy', 'Li Maksim').
parent('Lim Tatiana', 'Li Maksim').
parent('Li Dmitriy', 'Li Kiril').
parent('Lim Tatiana', 'Li Kiril').
parent('Sin Tok Un', 'Sin Men Sik').
parent('Sin Ok Suni', 'Sin Men Sik').
parent('Sin Tok Un', 'Sin U Sik').
parent('Sin Ok Suni', 'Sin U Sik').
parent('Sin Men Sik', 'Sin Viktor').
parent('Kim Antonina', 'Sin Viktor').
parent('Sin Men Sik', 'Sin Aleksandr').
parent('Kim Antonina', 'Sin Aleksandr').
parent('Styazhkina Nina', 'Styazhkina Elena').
parent('Styazhkina Nina', 'Styazhkin Sergey').
parent('Lim Nikolay', 'Lim Kiril').
parent('Lim Oksana', 'Lim Kiril').
parent('Lim Nikolay', 'Lim Roman').
parent('Lim Oksana', 'Lim Roman').
parent('Lim Nikolay', 'Lim Nikolay jr').
parent('Lim Oksana', 'Lim Nikolay jr').
parent('Sin Viktor', 'Sin Albert').
parent('Kim Elena', 'Sin Albert').
parent('Lim En Que', 'Lim Ten Sun').
parent('Tsoi Ki Suki', 'Lim Ten Sun').
parent('Lim En Que', 'Lim Ten Chury').
parent('Tsoi Ki Suki', 'Lim Ten Chury').
parent('Lim En Que', 'Lim En Se').
parent('Tsoi Ki Suki', 'Lim En Se').
parent('Lim Ten Sun', 'Kim Viktor').
parent('Lim Ten Sun', 'Kim Galina').
parent('I Sen Do', 'I Ok Suni').
parent('Che Ki Suki', 'I Ok Suni').
parent('I Sen Do', 'I Su Boki').
parent('Che Ki Suki', 'I Su Boki').
parent('I Su Boki', 'I Mikhail').
parent('I Su Boki', 'I Aleksandr').
parent('I Su Boki', 'I Sergey').

mother(X, Y) :-
	parent(X, Y),
	sex(X, f).

father(X, Y) :-
	parent(X, Y),
	sex(X, m).

son(X, Y) :-
	parent(Y, X),
	sex(X, m).

daughter(X, Y) :-
	parent(Y, X),
	sex(X, f).

brother(X, Y) :-
	parent(Q, X),
	parent(Q, Y),
	sex(Q, m),
	sex(X, m),
	not(X = Y).

sister(X, Y) :-
	parent(Q, X),
	parent(Q, Y),
	sex(Q, m),
	sex(X, f),
	not(X = Y).

% Task 3 Brother-in-law

husband(X, Y) :-
	parent(X, Q),
	parent(Y, Q),
	not(X = Y),
	sex(X, m), !.

brother_in_law(X, Y) :-
	husband(Q, Y),
	brother(X, Q).

% Task 4 Relative

move(Vert1, Vert2) :-
	brother(Vert2, Vert1).
move(Vert1, Vert2) :-
	sister(Vert2, Vert1).
move(Vert1, Vert2) :-
	parent(Vert2, Vert1).
move(Vert1, Vert2) :-
	parent(Vert1, Vert2).

prolong([X|T], [Y, X|T]) :-
	move(X, Y), not(member(Y, [X|T])).

depth_id([Finish|T], Finish, [Finish|T], 0).
depth_id(Path, Finish, R, N) :-
    N > 0,
    prolong(Path, NewPath),
    N1 is N - 1,
    depth_id(NewPath, Finish, R, N1).

search_id(Start, Finish, Path, DepthLimit) :-
    depth_id([Start], Finish, Res, DepthLimit),
    reverse(Res, Path).

search_id(Start, Finish, Path) :-
	generate(1, Level),
	search_id(Start, Finish, Path, Level).

generate(X,X).
generate(Y,X) :- Y<10, 
		Y1 is Y+1, 			generate(Y1,X).

short_relative('mother', X, Y) :- mother(X, Y).
short_relative('father', X, Y) :- father(X, Y).
short_relative('son', X, Y) :- son(X, Y).
short_relative('daughter', X, Y) :- daughter(X, Y).
short_relative('brother', X, Y) :- brother(X, Y).
short_relative('sister', X, Y) :- sister(X, Y).

path_transform([A, B|T], [R|X]) :-
	short_relative(R, A, B),
	path_transform([B|T], X).
path_transform([_], []).

relative(Rel, Human1, Human2) :-
	search_id(Human1, Human2, Path),
	path_transform(Path, Rel).

% Task 5

% Syntax Analizer
question(question(R)) --> general_q(R).
question(question(R)) --> special_q(R).
question(question(R)) --> subject_q(R).

general_q(general_q(A, B, C, D)) --> help_verb(A), subject(B), predicate(C), addition(D).
special_q(special_q(A, B, C, D, E, F)) --> special_question(A), addition(B), help_verb(C), subject(D), predicate(E), addition(F).
subject_q(subject_q(A, B, C)) --> special_question(A), help_verb(B), addition(C).

subject(subject(A)) --> noun(A).
subject(subject(A)) --> pronoun(A).

addition(addition(A)) --> noun(A).
addition(addition(A)) --> pronoun(A).
addition(addition(A, B, C)) --> noun(A), preposition(B), noun(C).
addition(addition(A, B, C)) --> noun(A), preposition(B), pronoun(C).
addition(addition(A, B, C)) --> preposition(A), noun(B), ['and'], noun(C).
addition(addition([])) --> [].

predicate(predicate(R)) --> verb(R).
predicate(predicate([])) --> [].

help_verb(help_verb(S)) 				--> {member(S, ['Is', 'is', 'Does', 'does'])}, [S].
special_question(special_question(S))	--> {member(S, ['Who', 'How many', 'What'])}, [S].
preposition(preposition(S)) 			--> {member(S, ['of', 'for', 'between'])}, [S].
noun(noun(S)) 							--> {member(S, ['father', 'mother', 'sister', 'sisters', 'brother', 'brothers', 'son', 'sons', 'daughter', 'daughters', 'relationship'])}, [S].
noun(noun(S)) 							--> {sex(S, _)}, [S].
verb(verb(S)) 							--> {member(S, ['have', 'is'])}, [S].
pronoun(pronoun(S))						--> {member(S, ['him', 'her', 'he', 'she'])}, [S].

build_ast(Lst, Tree) :-
	question(Tree, Lst, []).

get_rel(Lst) :-
	build_ast(
		Lst, question(general_q(
				help_verb('Is'), subject(noun(X)), predicate(_), addition(noun(Rel), preposition(_), noun(Y))))
		),
		asserta(person(X)),
		relative([Rel], X, Y).

get_rel(Lst) :-
	build_ast(
		Lst, question(general_q(
				help_verb('Is'), subject(noun(X)), predicate(_), addition(noun(Rel), preposition(_), pronoun('him'))))
		),
		catch(person(P), _, fail),
		sex(P, m),
		relative([Rel], X, P).

get_rel(Lst) :-
	build_ast(
		Lst, question(general_q(
				help_verb('Is'), subject(noun(X)), predicate(_), addition(noun(Rel), preposition(_), pronoun('her'))))
		),
		catch(person(P), _, fail),
		sex(P, f),
		relative([Rel], X, P).

get_rel(Lst) :-
	build_ast(
		Lst, question(special_q(
			special_question('How many'), addition(noun(Rel)), help_verb(_), subject(noun(X)), predicate(verb(_)), addition(_)))
		),
		asserta(person(X)),
		concat(NewRel, Tmp, Rel), string_length(Tmp, 1),
		findall(Hum, relative([NewRel], Hum, X), L),
		length(L, Ans),
		write(X), write(' has '), write(Ans), write(' '), write(Rel), nl. 

get_rel(Lst) :-
	build_ast(
		Lst, question(special_q(
			special_question('How many'), addition(noun(Rel)), help_verb(_), subject(pronoun('he')), predicate(_), addition(_)))
		),
		catch(person(P), _, fail),
		sex(P, m),
		concat(NewRel, Tmp, Rel), string_length(Tmp, 1),
		findall(Hum, relative([NewRel], Hum, P), L),
		length(L, Ans),
		write('He'), write(' has '), write(Ans), write(' '), write(Rel), nl.

get_rel(Lst) :-
	build_ast(
		Lst, question(special_q(
			special_question('How many'), addition(noun(Rel)), help_verb(_), subject(pronoun('she')), predicate(_), addition(_)))
		),
		catch(person(P), _, fail),
		sex(P, f),
		concat(NewRel, Tmp, Rel), string_length(Tmp, 1),
		findall(Hum, relative([NewRel], Hum, P), L),
		length(L, Ans),
		write('She'), write(' has '), write(Ans), write(' '), write(Rel), nl.

get_rel(Lst) :-
	build_ast(
		Lst, question(special_q(
			special_question('What'), addition(_), help_verb('is'), subject(noun('relationship')), predicate(_), addition(preposition(_), noun(X), noun(Y))))
		),
		asserta(person(Y)),
		relative(Ans, X, Y),
		write('Relationship between '), write(X), write(' and '), write(Y), write(' is '),
		write(Ans), nl.
		
get_rel(Lst) :-
	build_ast(
		Lst, question(subject_q(
			special_question('Who'), help_verb(_), addition(noun(Rel), preposition(_), noun(Y))))
		),
		asserta(person(Y)),
		relative([Rel], Ans, Y),
		write(Rel), write(' for '), write(Y), write(' is '), write(Ans), nl.

get_rel(Lst) :-
	build_ast(
		Lst, question(subject_q(
			special_question('Who'), help_verb(_), addition(noun(Rel), preposition(_), pronoun('him'))))
		),
		person(Y),
		sex(Y, m),
		relative([Rel], Ans, Y),
		write(Rel), write(' for '), write('him'), write(' is '), write(Ans), nl.

get_rel(Lst) :-
	build_ast(
		Lst, question(subject_q(
			special_question('Who'), help_verb(_), addition(noun(Rel), preposition(_), pronoun('her'))))
		),
		person(Y),
		sex(Y, f),
		relative([Rel], Ans, Y),
		write(Rel), write(' for '), write('her'), write(' is '), write(Ans), nl.
