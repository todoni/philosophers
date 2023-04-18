# philosophers
[식사하는 철학자 문제](https://ko.wikipedia.org/wiki/%EC%8B%9D%EC%82%AC%ED%95%98%EB%8A%94_%EC%B2%A0%ED%95%99%EC%9E%90%EB%93%A4_%EB%AC%B8%EC%A0%9C)를 시뮬레이션 하는 프로그램입니다.
## How To Build
```bash
$ git clone https://github.com/todoni/philosophers

$ cd philosophers/

$ cd philo/

$ make
```
## Usage
다음의 인자가 필요합니다.
- number_of_philosophers: 철학자의 수
- time_to_die: 죽지 않기 위한 시간
- time_to_eat: 밥을 먹는데 걸리는 시간
- time_to_sleep: 잠을 자는데 걸리는 시간
- [number_of_times_each_philosophers_must_eat]: 각 철학자들이 식사를 해야 하는 최소 횟수(optional)
<br>

```bash
$ ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosophers_must_eat]
```
