# SIMPLE CHAT SERVER
Simple Chat Server është një aplikacion i thjeshtë server i cili lejon dy e më shumë klientë të lidhen dhe të komunikojnë me njëri tjetrin
në një mjedis të ngjashëm me “chatrooms”. Ai është programuar duke përdorur gjuhën C dhe është i krijuar për tu ekzekutuar në sistemin operativ.

# Struktura dhe përshkrimi i Simple Chat Server
Server.c:
Së pari hapim lidhjen me server përmes komandës ./server <key>, dhe do të na shfaqen klientët që janë lidhur në server të cilët do të komunikojnë me njëri-tjetrin.

Client.c:
Pas lidhjes me server, hapim terminalet në bazë të numrit të klientave që do të lidhen dhe qasemi në server përmes komandës ./client <key> <name>
Ky file paraqet client-side të kodit. Kur ekzekutohet, kërkon nga useri të shtyp çelësin dhe emrin e klientit. Pastaj krijon lidhje me serverin duke u lidhur përmes message queue duke përdorur çelësin.
Nga klienti krijohen 2 threads: një për dërgim mesazhesh, dhe një për marrje mesazhesh:
1. send_msg_thread
2. recv_msg_thread

# Dokumentimi i projektit 
[Dokumentimi:]

# Grupi punues:
•[Dafina Balaj](https://github.com/dafinabalaj) 

•[Dafina Sadiku](https://github.com/dafiinaa) 

•[Dëshira Randobrava](https://github.com/d3shira) 

•[Besmira Berisha](https://github.com/Besmira75)


# Lënda 
Sisteme Operative
# Profesoreshë
Prof. As. Dr. Artan Mazrekaj
# Asistente
MSc. Dalinë Vranovci

