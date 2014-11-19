var app = angular.module('data-viz-proj', ['ui.bootstrap']);

app.controller('MainCtrl', function($scope) {
  $scope.images = {}
  $scope.createImages = function(){
    for (var i=0; i<216; i++){
        $scope.images[i] = '/images/' + i + '.jpg'
    }
    console.log($scope.images);
  }

  $scope.showPath = function (imageNumber) {
    return $scope.images[imageNumber];
  }

})
