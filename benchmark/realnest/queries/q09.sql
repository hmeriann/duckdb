SELECT 
  list_aggregate(tau_pt, 'favg'),
  list_aggregate(tau_eta, 'favg'),
  list_aggregate(jet_pt, 'favg'),
  list_aggregate(jet_eta, 'favg'),
  list_aggregate(muon_pt, 'favg'),
  list_aggregate(muon_eta, 'favg'),
  list_aggregate(el_pt, 'favg'),
  list_aggregate(el_eta, 'favg'),
  list_aggregate(ph_pt, 'favg'),
  list_aggregate(ph_eta, 'favg')  
FROM singleMu;