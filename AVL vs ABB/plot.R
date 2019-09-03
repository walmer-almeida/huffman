graf<- read.table("CompData.txt", header = TRUE, sep = "\t")
plot(graf$Comparações_ABB~graf$Tamanho_ABB, main="[Árvore Balanceada X Árvore Desbalanceada", xlab="Valores", ylab="ComparaÃ§Ãµes", xlim= c(0,5000), ylim= c(0, 40), col="green")
points(graf$Comparações_AVL~graf$Tamanho_AVL, col="purple")
legend("topleft", legend=c("Comparações ABB", "Comparações AVL"),
       col=c("green", "purple"), pch=1)