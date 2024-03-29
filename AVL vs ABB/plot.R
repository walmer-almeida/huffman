graf<- read.table("CompData.txt", header = TRUE, sep = "\t")
plot(graf$Compara��es_ABB~graf$Tamanho_ABB, main="[�rvore Balanceada X �rvore Desbalanceada", xlab="Valores", ylab="Comparações", xlim= c(0,5000), ylim= c(0, 40), col="green")
points(graf$Compara��es_AVL~graf$Tamanho_AVL, col="purple")
legend("topleft", legend=c("Compara��es ABB", "Compara��es AVL"),
       col=c("green", "purple"), pch=1)