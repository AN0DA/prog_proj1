# Projekt dodatkowy - statystyki ankiety

W wolnej chwili napisałem również prosty projekt poboczny.

> Zadania na ocenę 6:
> (...)
> 10. Napisz program udostępniający opcję sumowania wyników ankiety, w której mogą wystąpić trzy różne wartości. Dane wejściowe wprowadzane do programu to pytanie ankietowe oraz trzy możliwe odpowiedzi. Pierwszej odpowiedzi przypisywana jest wartość 1, drugiej 2, a trzeciej 3. Odpowiedzi są sumowane do chwili, w której użytkownik wprowadzi 0 – wtedy program powinien pokazać wyniki ankiety. Postaraj się wygenerować wykres słupkowy, pokazujący wyniki przeskalowane w taki sposób, aby zmieściły się na ekranie bez względu na liczbę udzielonych odpowiedzi.

Napisałem go możliwie najprościej i przy użyciu zaledwie trzech zewnętrznych bibliotek. Zgodnie z założeniem wczytuje on wartości po czym wyświetla wyniki w postaci poziomego wykresu słupkowego, który skaluje się odpowiednio do wielkości okna.
Nie został zaimplementowany żaden interfejs graficzny, słupki powstają z opdpowiednich znaków Unicode. Wadą jest to, że program działa tylko na systemie Windows.