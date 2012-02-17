acd() {
  if [ x$1 = x ]; then
    echo "Usage: acd <pattern>"
    return 1
  fi
  cd "$(qf $1 | sed 1q)"
}
