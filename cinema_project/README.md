# Symulator Kina (C++)

Konsolowy symulator zarządzania kinem: konfiguracja sal i filmów, repertuar na dany dzień, sprzedaż biletów z wyborem miejsc, zwroty zatwierdzane przez administratora oraz oceny filmów (1–5).

## Wymagania

- Linux + g++ z obsługą C++17.
- make (wykorzystanie pliku Makefile).

## Kompilacja

```bash
make
```
Dla bezpieczeństwa, przy pierwszym uruchomieniu:

```bash
make clean && make
```

## Uruchomienie

Tryb interaktywny:

```bash
./cinema
```

W dowolnym momencie możesz wyświetlić pomoc:

```bash
help
```

lub

```bash
-help
```

(różne komendy w zależności od aktualnej roli)

Zakończenie programu:

```bash
exit
```

## Trwały zapis danych

Program zapisuje stan do folderu data/ (pliki .txt) i wczytuje go przy następnym uruchomieniu, dzięki czemu dane nie resetują się po zamknięciu programu.

Zapisywane są m.in.:

- sale, filmy, seanse, bilety, oceny,
- stan kasy (balance, dailyIncome),
- liczniki ID i stan czasu dnia. ​

Jeśli chcesz zacząć od zera, usuń folder data/ lub jego zawartość.

## Logowanie

Po starcie jesteś w trybie guest.

Logowanie administratora:

- `login admin admin`

Hasło admina jest stałe: "admin".


Logowanie klienta:

- `login client <nick>`

<nick> to dowolny nickname (konto tworzy się automatycznie).


Wylogowanie (admin lub klient):

- `logout`

## Komendy administratora

Najważniejsze komendy:

- add_hall <id> <rows> <cols> – dodaje salę, np. add_hall Alfa 5 6.
- add_movie <title> <durationMin> <ageRating> – dodaje film, np. add_movie Shrek 120 6.
- add_screening <title> <hallId> <HH:MM> <price> – dodaje seans na dziś (warunek: dodana co najmniej jedna sala i film), 
np. add_screening Shrek Alpha 18:30 20,50 (format ceny poprawny zarówno z ",", jak i ".")
- start_day / end_day – start / koniec dnia (sprzedaż i zwroty działają tylko przy aktywnym dniu).
- repertoire – pokazuje repertuar (seanse na dziś).
- refunds – lista próśb o zwrot.
- decide_refund <ticketId> <0/1> – decyzja admina o zwrocie (1 - zaakceptuj, 0 - odrzuć).
- time <minutes> – przesuwa czas , np. time 120.
- money – pokazuje balans kina i dzienny przychód.

Typowy scenariusz:

Administrator loguje się na swoje konto:
- `login admin admin`

Dodaje dostępne hale, filmy i seanse:
- `add_hall Alfa 5 6`
- `add_movie Shrek 120 6`
- `add_screening Shrek Alpha 18:30 20,50`

Startuje dzień i kino zaczyna pracę
- `start_day`

## Komendy klienta

Najważniejsze komendy:
- repertoire - pokazuje repertuar (seanse na dziś).
- select_show <title> <hallId> <HH:MM> - Wybór filmu do zakupu biletu w formacie: film - ID sali - godzina rozpoczęcia. 
- buy <count> [lista_miejsc...] - Kupno biletu na film (po wybraniu filmu komenda select_show).
- mytickets - Wypisanie wszystkich biletów danego klienta (ID, seans, miejsce, cena, status)
- refund <ticketId> – prośba o zwrot (dzień musi być aktywny i do seansu musi zostać min. 60 min) - ticketId można odczytać pod komendą `mytickets`.
- rate <title> <1-5> – ocena filmu (tylko jeśli klient miał bilet na ten film), np. rate Shrek 5.
- time <minutes> – przesuwa czas , np. time 120.

Typowy scenariusz:

Klient ogląda repertuar i patrzy na interesujące go pozycje:
- `repertoire`

Potem wybiera film, na który chciałby zakupić bilety:
- `select_show Shrek Alfa 18:30`

Wyświetla mu się lista dostępnych miejsc, dzięki czemu może wybrać te, które go interesują:
- `buy 2 3C 3D`

Ewentualnie przed filmem może zwrócić bilet:
- `refund 1`

Lub ocenić film po jego obejrzeniu:
- `rate Shrek 5`

## Uwagi

- Tytuły filmów w komendach trzeba podawać jako pojedynczy token (bez spacji), np. The_Dark_Knight.
- Jedna sala może grać tylko jeden seans dziennie.