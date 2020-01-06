# prog_proj1

Projekt przygotwany na przedmiot Programowanie na koniec I semestru 3 klasy (2019/2020).

Aplikacja została zrobiona przy użyciu Qt 5.14.0 oraz MS SQL Server 2017 Standard.

PRZED URUCHOMIENIEM:
1. Na serwerze SQL Server utwórz bazę "Amnesty2019" [`CREATE DATABASE Amnesty2019`]
2. Uruchom script.sql jako kwerendę
3. Przydziel użytkownikowi odpowiednie uprawnienia dostępu do bazy [Amnesty2019]
4. Skonfiguruj połączenie ODBC:
    Nazwa: Amnesty2019
    Domyślna baza danych: Amnesty2019
Po skompilowaniu aplikacja powinna samoistnie połączyć się z serwerem

FUNKCJONALNOŚCI:
  - Rejestrowanie wejścia i wyjścia uczniów
  - Rejestrowanie napisanych listów
  - Kontrola ilości osób na salach
  - Wyświetlanie podstawowych statystyk
  - Możliwość manualnej edycji tabel
